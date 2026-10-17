#include <xaudio2.h>
#include <vector>
#include "Audio.h"
#include "Global.h"
#include "Debug.h"

namespace Audio
{
	//XAudio本体
	IXAudio2* pXAudio = nullptr;

	//マスターボイス
	IXAudio2MasteringVoice* pMasteringVoice = nullptr;


	//ファイル毎に必要な情報
	struct AudioData
	{
		//サウンド情報
		XAUDIO2_BUFFER buf = {};

		//ソースボイス
		IXAudio2SourceVoice** pSourceVoice = nullptr;

		//同時再生最大数
		int svNum;

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
	if (pXAudio != nullptr)
	{
		return S_OK;
	}

	//XAudio本体を作る
	HRESULT hr = XAudio2Create(&pXAudio);
	if (FAILED(hr))
	{
		pXAudio = nullptr;
		AllRelease();
		Debug::Fatal("XAudio2を初期化できません。" + Debug::HResult(hr));
		return hr;
	}

	//マスターボイスを作る
	hr = pXAudio->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		pMasteringVoice = nullptr;
		AllRelease();
		Debug::Fatal("オーディオの出力先（マスターボイス）を作成できません。" + Debug::HResult(hr));
		return hr;
	}

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

	//指定したバイト数を読み込む（足りなければfalse）
	auto readExact = [&](void* pDest, DWORD bytes) -> bool
	{
		DWORD readBytes = 0;
		return ReadFile(hFile, pDest, bytes, &readBytes, NULL) && readBytes == bytes;
	};

	//ファイルの残りのバイト数
	auto remainBytes = [&]() -> DWORD
	{
		DWORD position = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		return position <= fileSize ? fileSize - position : 0;
	};

	//RIFFヘッダ
	char riffId[4];
	DWORD riffSize;
	char waveId[4];
	if (!readExact(riffId, 4) || !readExact(&riffSize, 4) || !readExact(waveId, 4) ||
		memcmp(riffId, "RIFF", 4) != 0 || memcmp(waveId, "WAVE", 4) != 0)
	{
		return fail("WAVEファイルではありません");
	}

	//チャンクを順番にたどって、fmt（フォーマット）とdata（波形データ）を探す
	//それ以外のチャンクは読み飛ばす
	//https://learn.microsoft.com/ja-jp/windows/win32/api/mmeapi/ns-mmeapi-waveformatex
	WAVEFORMATEXTENSIBLE fmt = {};
	bool isFormatFound = false;
	char* pBuffer = nullptr;
	DWORD dataSize = 0;
	while (pBuffer == nullptr)
	{
		//次のチャンクのIDとサイズ
		char chunkId[4];
		DWORD chunkSize;
		if (!readExact(chunkId, 4) || !readExact(&chunkSize, 4))
		{
			break;	//ファイルの終わり
		}

		//チャンクの中身がファイルの残りに収まっているか
		if (chunkSize > remainBytes())
		{
			return fail("ファイルが途中で切れています");
		}

		//フォーマット
		if (memcmp(chunkId, "fmt ", 4) == 0)
		{
			if (chunkSize < 16)	return fail("fmtチャンクが不正です");

			//先頭の必要な分だけ読み、残りは読み飛ばす
			DWORD readSize = chunkSize < sizeof(fmt) ? chunkSize : (DWORD)sizeof(fmt);
			if (!readExact(&fmt, readSize))	return fail("fmtチャンクを読み込めません");
			isFormatFound = true;
			SetFilePointer(hFile, (LONG)(chunkSize - readSize + (chunkSize & 1)), NULL, FILE_CURRENT);
		}

		//波形データ
		else if (memcmp(chunkId, "data", 4) == 0)
		{
			if (!isFormatFound)	return fail("fmtチャンクがdataチャンクより前にありません");

			pBuffer = new char[chunkSize];
			if (!readExact(pBuffer, chunkSize))
			{
				delete[] pBuffer;
				return fail("波形データを読み込めません");
			}
			dataSize = chunkSize;
		}

		//使わないチャンクは読み飛ばす
		else
		{
			SetFilePointer(hFile, (LONG)(chunkSize + (chunkSize & 1)), NULL, FILE_CURRENT);
		}
	}

	if (pBuffer == nullptr)	return fail(isFormatFound ? "dataチャンクが見つかりません" : "fmtチャンクが見つかりません");


	AudioData ad;

	ad.fileName = fileName;

	ad.buf.pAudioData = (BYTE*)pBuffer;
	ad.buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop)	ad.buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	ad.buf.AudioBytes = dataSize;


	ad.pSourceVoice = new IXAudio2SourceVoice * [svNum]();
	ad.svNum = svNum;

	for (int i = 0; i < svNum; i++)
	{
		HRESULT hr = pXAudio->CreateSourceVoice(&ad.pSourceVoice[i], &fmt.Format);
		if (FAILED(hr))
		{
			//作りかけのものを解放して、失敗にする
			for (int j = 0; j < i; j++)
			{
				ad.pSourceVoice[j]->DestroyVoice();
			}
			SAFE_DELETE_ARRAY(ad.pSourceVoice);
			SAFE_DELETE_ARRAY(ad.buf.pAudioData);
			return fail("再生用のボイスを作成できません " + Debug::HResult(hr));
		}
	}
	audioDatas.push_back(ad);

	return (int)audioDatas.size() - 1;
}

//再生
void Audio::Play(int ID, float volume)
{
	if (!IsValid(ID))	return;

	for (int i = 0; i < audioDatas[ID].svNum; i++)
	{
		XAUDIO2_VOICE_STATE state;
		audioDatas[ID].pSourceVoice[i]->GetState(&state);
		audioDatas[ID].pSourceVoice[i]->SetVolume(volume);

		if (state.BuffersQueued == 0)
		{
			audioDatas[ID].pSourceVoice[i]->SubmitSourceBuffer(&audioDatas[ID].buf);
			audioDatas[ID].pSourceVoice[i]->Start();
			break;
		}
	}
}


void Audio::Stop(int ID)
{
	if (!IsValid(ID))	return;

	for (int i = 0; i < audioDatas[ID].svNum; i++)
	{
		audioDatas[ID].pSourceVoice[i]->Stop();
		audioDatas[ID].pSourceVoice[i]->FlushSourceBuffers();
	}
}

//シーンごとの解放
//ロードしたサウンドだけを解放する（XAudio本体とCOMは解放しない）
void Audio::Release()
{
	for (int i = 0; i < audioDatas.size(); i++)
	{
		for (int j = 0; j < audioDatas[i].svNum; j++)
		{
			audioDatas[i].pSourceVoice[j]->DestroyVoice();
		}
		SAFE_DELETE_ARRAY(audioDatas[i].pSourceVoice);
		SAFE_DELETE_ARRAY(audioDatas[i].buf.pAudioData);
	}
	audioDatas.clear();
}

//本体の解放
//アプリケーションの終了時に一度だけ呼ぶ
//初期化に失敗した作りかけの状態でも安全に呼べる
void Audio::AllRelease()
{
	//ロード済みのサウンドを先に解放する（ソースボイスはマスターボイスより先に破棄する）
	Release();

	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}

	if (pXAudio)
	{
		pXAudio->Release();
		pXAudio = nullptr;
	}

}
