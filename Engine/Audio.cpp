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

	//チャンク構造体
	struct Chunk
	{
		char	id[5] =""; 			// ID
		unsigned int	size = 0;	// サイズ
	};

	//ファイルを開く
	HANDLE hFile;
	hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwBytes = 0;

	Chunk riffChunk = { 0 };
	ReadFile(hFile, &riffChunk.id, 4, &dwBytes, NULL);
	ReadFile(hFile, &riffChunk.size, 4, &dwBytes, NULL);

	char wave[4] = "";
	ReadFile(hFile, &wave, 4, &dwBytes, NULL);

	Chunk formatChunk = { 0 };
	while (formatChunk.id[0] != 'f') {
		ReadFile(hFile, &formatChunk.id, 4, &dwBytes, NULL);
	}
	ReadFile(hFile, &formatChunk.size, 4, &dwBytes, NULL);


	//フォーマットを読み取る
	//https://learn.microsoft.com/ja-jp/windows/win32/api/mmeapi/ns-mmeapi-waveformatex
	WAVEFORMATEX fmt;
	ReadFile(hFile, &fmt.wFormatTag, 2, &dwBytes, NULL);		//形式
	ReadFile(hFile, &fmt.nChannels, 2, &dwBytes, NULL);			//チャンネル（モノラル/ステレオ）
	ReadFile(hFile, &fmt.nSamplesPerSec, 4, &dwBytes, NULL);	//サンプリング数
	ReadFile(hFile, &fmt.nAvgBytesPerSec, 4, &dwBytes, NULL);	//1秒あたりのバイト数
	ReadFile(hFile, &fmt.nBlockAlign, 2, &dwBytes, NULL);		//ブロック配置
	ReadFile(hFile, &fmt.wBitsPerSample, 2, &dwBytes, NULL);	//サンプル当たりのビット数



	//波形データの読み込み
	Chunk data = { 0 };
	while (true)
	{
		//次のデータのIDを調べる
		ReadFile(hFile, &data.id, 4, &dwBytes, NULL);

		//「data」だったらループを抜けて次に進む
		if (strcmp(data.id, "data") == 0)
			break;

		//それ以外の情報ならサイズ調べて読み込む→使わない
		else
		{
			//サイズ調べて
			ReadFile(hFile, &data.size, 4, &dwBytes, NULL);
			char* pBuffer = new char[data.size];

			//無駄に読み込む
			ReadFile(hFile, pBuffer, data.size, &dwBytes, NULL);
		}
	}

	//データチャンクのサイズを取得
	ReadFile(hFile, &data.size, 4, &dwBytes, NULL);

	//波形データを読み込む
	char* pBuffer = new char[data.size];
	ReadFile(hFile, pBuffer, data.size, &dwBytes, NULL);
	CloseHandle(hFile);


	AudioData ad;

	ad.fileName = fileName;

	ad.buf.pAudioData = (BYTE*)pBuffer;
	ad.buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop)	ad.buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	ad.buf.AudioBytes = data.size;


	ad.pSourceVoice = new IXAudio2SourceVoice * [svNum];

	for (int i = 0; i < svNum; i++)
	{
		pXAudio->CreateSourceVoice(&ad.pSourceVoice[i], &fmt);
	}
	ad.svNum = svNum;
	audioDatas.push_back(ad);

	//SAFE_DELETE_ARRAY(pBuffer);

	return (int)audioDatas.size() - 1;
}

//再生
void Audio::Play(int ID, float volume)
{
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
