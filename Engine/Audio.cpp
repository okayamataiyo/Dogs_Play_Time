#include <xaudio2.h>
#include <vector>
#include <memory>
#include <new>
#include <wrl/client.h>
#include "Audio.h"
#include "WavParser.h"
#include "Global.h"
#include "Debug.h"

namespace Audio
{
	//ボイスを破棄する削除関数（deleteではなく、DestroyVoice()で破棄する決まり）
	struct VoiceDestroyer
	{
		void operator()(IXAudio2Voice* pVoice) const
		{
			if (pVoice != nullptr)
			{
				pVoice->DestroyVoice();
			}
		}
	};

	//XAudio本体
	Microsoft::WRL::ComPtr<IXAudio2> pXAudio;

	//マスターボイス
	std::unique_ptr<IXAudio2MasteringVoice, VoiceDestroyer> pMasteringVoice;


	//ファイル毎に必要な情報
	struct AudioData
	{
		//波形データ
		//（メンバーは宣言の逆順に解放される。ソースボイスを破棄してから解放したいので、ボイスより先に宣言する）
		std::unique_ptr<BYTE[]> data;

		//サウンド情報（波形データの場所は、上のdataを指す）
		XAUDIO2_BUFFER buf = {};

		//ソースボイス（同時に鳴らす最大数だけ持つ）
		std::vector<std::unique_ptr<IXAudio2SourceVoice, VoiceDestroyer>> voices;

		//ファイル名
		std::string fileName;
	};
	std::vector<AudioData>	audioDatas;

	//指定した番号のサウンドが使えるか（Loadで読み込めたもの）
	static bool IsValid(int ID)
	{
		return ID >= 0 && ID < (int)audioDatas.size();
	}
}

//初期化
//アプリケーションの起動時に一度だけ呼ぶ（シーン切替のたびには呼ばない）
//COM（CoInitializeEx）は、呼ぶ前にMainが初期化しておくこと
//すでに初期化済みの場合は何もしない
//戻値：成功ならS_OK、失敗なら失敗したAPIのHRESULT（原因は致命的なエラーとして記録済みで、作りかけのものは解放済み）
HRESULT Audio::Initialize()
{
	if (pXAudio.Get() != nullptr)
	{
		return S_OK;
	}

	//XAudio本体を作る
	HRESULT hr = XAudio2Create(pXAudio.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		pXAudio.Reset();
		AllRelease();
		Debug::Fatal("XAudio2を初期化できません。" + Debug::HResult(hr));
		return hr;
	}

	//マスターボイスを作る
	IXAudio2MasteringVoice* pMaster = nullptr;
	hr = pXAudio->CreateMasteringVoice(&pMaster);
	if (FAILED(hr))
	{
		AllRelease();
		Debug::Fatal("オーディオの出力先（マスターボイス）を作成できません。" + Debug::HResult(hr));
		return hr;
	}

	pMasteringVoice.reset(pMaster);

	return S_OK;
}

//サウンドファイル(.wav）をロード
int Audio::Load(std::string fileName, bool isLoop, int svNum)
{
	//すでに同じファイルを使ってないかチェック
	//for (int i = 0; i < audioDatas.size(); i++)
	//{
	//	if (audioDatas[i].fileName == fileName)
	//	{
	//		return i;
	//	}
	//}

	//読み込めなかったときは、警告を記録して-1を返す
	auto fail = [&](const std::string& reason) -> int
	{
		Debug::Warning("音声を読み込めません。鳴らさずに続行します: " + fileName + " (" + reason + ")");
		return -1;
	};

	if (pXAudio == nullptr)	return fail("オーディオが初期化されていません");
	if (svNum < 1)			return fail("同時に鳴らす最大数が不正です");

	//ファイルを開く（どの出口でも閉じる）
	struct FileCloser
	{
		HANDLE handle;
		~FileCloser() { if (handle != INVALID_HANDLE_VALUE)	CloseHandle(handle); }
	};
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return fail("ファイルを開けません " + Debug::HResult(HRESULT_FROM_WIN32(GetLastError())));
	}
	FileCloser closer = { hFile };

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE)	return fail("ファイルの大きさを調べられません");

	//ファイルの中身をすべて読み込む
	std::unique_ptr<BYTE[]> pFile(new (std::nothrow) BYTE[fileSize]);
	if (pFile == nullptr)	return fail("メモリが足りません");
	DWORD readBytes = 0;
	if (!ReadFile(hFile, pFile.get(), fileSize, &readBytes, NULL) || readBytes != fileSize)
	{
		return fail("ファイルを読み込めません");
	}

	//チャンクを順番にたどって、fmt（フォーマット）とdata（波形データ）を探す
	WavParser::WavInfo wav;
	std::string reason;
	if (!WavParser::Parse(pFile.get(), fileSize, &wav, &reason))
	{
		return fail(reason);
	}

	//波形データだけを取り出す
	WAVEFORMATEXTENSIBLE fmt = wav.format;
	DWORD dataSize = wav.dataSize;
	std::unique_ptr<BYTE[]> pBuffer(new BYTE[dataSize]);
	memcpy(pBuffer.get(), pFile.get() + wav.dataOffset, dataSize);

	AudioData ad;

	ad.fileName = fileName;

	ad.data = std::move(pBuffer);
	ad.buf.pAudioData = ad.data.get();
	ad.buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop)	ad.buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	ad.buf.AudioBytes = dataSize;


	//ソースボイスを、同時に鳴らす最大数だけ作る（途中で失敗したら、作りかけのものはadと一緒に解放される）
	for (int i = 0; i < svNum; i++)
	{
		IXAudio2SourceVoice* pVoice = nullptr;
		HRESULT hr = pXAudio->CreateSourceVoice(&pVoice, &fmt.Format);
		if (FAILED(hr))
		{
			return fail("再生用のボイスを作成できません " + Debug::HResult(hr));
		}
		ad.voices.emplace_back(pVoice);
	}
	audioDatas.push_back(std::move(ad));

	return (int)audioDatas.size() - 1;
}

//再生
void Audio::Play(int ID, float volume)
{
	if (!IsValid(ID))	return;

	for (size_t i = 0; i < audioDatas[ID].voices.size(); i++)
	{
		XAUDIO2_VOICE_STATE state;
		audioDatas[ID].voices[i]->GetState(&state);
		audioDatas[ID].voices[i]->SetVolume(volume);

		if (state.BuffersQueued == 0)
		{
			audioDatas[ID].voices[i]->SubmitSourceBuffer(&audioDatas[ID].buf);
			audioDatas[ID].voices[i]->Start();
			break;
		}
	}
}


void Audio::Stop(int ID)
{
	if (!IsValid(ID))	return;

	for (size_t i = 0; i < audioDatas[ID].voices.size(); i++)
	{
		audioDatas[ID].voices[i]->Stop();
		audioDatas[ID].voices[i]->FlushSourceBuffers();
	}
}

//シーンごとの解放
//ロードしたサウンドだけを解放する（XAudio本体とCOMは解放しない）
void Audio::Release()
{
	//ソースボイスを破棄してから、波形データを解放する（AudioDataのメンバーの順序で、この順になる）
	audioDatas.clear();
}

//本体の解放
//アプリケーションの終了時に一度だけ呼ぶ
//初期化に失敗した作りかけの状態でも安全に呼べる
void Audio::AllRelease()
{
	//ロード済みのサウンドを先に解放する（ソースボイスはマスターボイスより先に破棄する）
	Release();

	//マスターボイス → XAudio本体の順に解放する
	pMasteringVoice.reset();
	pXAudio.Reset();

}
