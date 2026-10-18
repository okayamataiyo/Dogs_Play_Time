#pragma once
#include <Windows.h>
#include <mmreg.h>
#include <string>

//-----------------------------------------------------------
//WAVファイル（RIFF形式）の中身を解析する
//メモリ上のバイト列だけを見るので、ファイルの読み込みやXAudioには依存しない
//-----------------------------------------------------------
namespace WavParser
{
	//解析した結果
	struct WavInfo
	{
		WAVEFORMATEXTENSIBLE	format;		//fmtチャンクの内容（fmtチャンクが短いときの足りない分は0）
		size_t					dataOffset;	//波形データの先頭の位置（ファイルの先頭からのバイト数）
		DWORD					dataSize;	//波形データのバイト数
	};

	//WAVファイルの中身を解析して、fmt（フォーマット）とdata（波形データ）の場所を調べる
	//チャンクを順番にたどり、fmtとdata以外のチャンクは読み飛ばす
	//引数：pFile		ファイルの中身（fileSizeが0ならnullptrでもよい）
	//引数：fileSize	ファイルのバイト数
	//引数：pInfo		解析結果の格納先（失敗したときは変わらない）
	//引数：pReason		失敗したときの原因の格納先（不要ならnullptr）
	//戻値：解析できたらtrue
	bool Parse(const BYTE* pFile, size_t fileSize, WavInfo* pInfo, std::string* pReason = nullptr);
};
