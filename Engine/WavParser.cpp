#include <cstring>
#include "WavParser.h"

//WAVファイルの中身を解析する
bool WavParser::Parse(const BYTE* pFile, size_t fileSize, WavInfo* pInfo, std::string* pReason)
{
	//解析できなかったときは、原因を返してfalseにする
	auto fail = [&](const char* reason) -> bool
	{
		if (pReason != nullptr)	*pReason = reason;
		return false;
	};

	if (pInfo == nullptr)	return fail("解析結果の格納先がありません");

	//今読んでいる位置（ファイルの先頭からのバイト数）。ファイルの終わりを越えない
	size_t position = 0;

	//指定したバイト数を読み込む（足りなければfalse）
	auto readExact = [&](void* pDest, size_t bytes) -> bool
	{
		if (fileSize - position < bytes)	return false;
		memcpy(pDest, pFile + position, bytes);
		position += bytes;
		return true;
	};

	//指定したバイト数を読み飛ばす（ファイルの終わりを越えるときは終わりで止める）
	auto skip = [&](size_t bytes)
	{
		position = (fileSize - position < bytes) ? fileSize : position + bytes;
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
	//それ以外のチャンクは読み飛ばす（チャンクの大きさが奇数のときは、1バイトの詰め物が付く）
	//https://learn.microsoft.com/ja-jp/windows/win32/api/mmeapi/ns-mmeapi-waveformatex
	WAVEFORMATEXTENSIBLE fmt = {};
	bool isFormatFound = false;
	while (true)
	{
		//次のチャンクのIDとサイズ
		char chunkId[4];
		DWORD chunkSize;
		if (!readExact(chunkId, 4) || !readExact(&chunkSize, 4))
		{
			break;	//ファイルの終わり
		}

		//チャンクの中身がファイルの残りに収まっているか
		if (chunkSize > fileSize - position)
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
			skip((size_t)(chunkSize - readSize) + (chunkSize & 1));
		}

		//波形データ
		else if (memcmp(chunkId, "data", 4) == 0)
		{
			if (!isFormatFound)	return fail("fmtチャンクがdataチャンクより前にありません");

			pInfo->format = fmt;
			pInfo->dataOffset = position;
			pInfo->dataSize = chunkSize;
			return true;
		}

		//使わないチャンクは読み飛ばす
		else
		{
			skip((size_t)chunkSize + (chunkSize & 1));
		}
	}

	return fail(isFormatFound ? "dataチャンクが見つかりません" : "fmtチャンクが見つかりません");
}
