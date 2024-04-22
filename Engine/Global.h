#pragma once
//インクルード
#include <string>
#include "Direct3D.h"

namespace
{
	enum class WINORLOSE
	{
		WIN = 0,
		LOSE,
	};

	int PlayerScore_[2] = { 0,0 };
	int winOrLose_ = (int)WINORLOSE::WIN;
}

//安全にメモリを開放するためのマクロ
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//0に初期化
#define ZERO_INITIALIZE(p) { p = 0;}

#define ZERO_XMFLOAT3_INITIALIZE(p){p = {0,0,0};}

//1に初期化
#define ONE_INITIALIZE(p) { p = 1;}

//引数で初期化
#define ARGUMENT_INITIALIZE(p,t){ p = t;}
static XMVECTOR vecUp = { 0.0f,1.0f,0.0f };
static XMVECTOR vecDown = { 0.0f,-1.0f,0.0f };
static XMVECTOR vecFront = { 0.0f,0.0f,1.0f };
static XMVECTOR vecBack = { 0.0f,0.0f,-1.0f };
static XMVECTOR vecRight = { 1.0f,0.0f,0.0f };
static XMVECTOR vecLeft = { -1.0f,0.0f,0.0f };
static XMVECTOR vecFrontUp = { 0.0f,1.0f,1.0f };
static XMVECTOR vecBackUp = { 0.0f,1.0f,-1.0f };
static XMVECTOR vecRightUp = { 1.0f,1.0f,0.0f };
static XMVECTOR vecLeftUp = { -1.0f,1.0f,0.0f };
static XMVECTOR vecBackLeft = { -1.0f,0.0f,-1.0f };
static XMFLOAT3 initZeroXMFLOAT3 = {0.0f,0.0f,0.0f};
static XMVECTOR initZeroXMVECTOR = { 0.0f,0.0f,0.0f,0.0f };
static float initOneFloat = { 1.0f };
static float initZeroFloat = { 0.0f };
static int initZeroInt = { 0 };
static int initOneInt = { 1 };
static int normalizationInt = { 1 };
static std::string soundFolderName = "Sound/";
static std::string soundModifierName = ".wav";
static std::string modelFolderName = "Model&Picture/";
static std::string modelModifierName = ".fbx";
static std::string debugCollisionName = "DebugCollision/";
static std::string imageModifierName = ".png";

// INIファイルからfloatを取得する関数
static float GetPrivateProfileFloat(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName)
{
	// ファイル名     
	TCHAR buffer[MAX_PATH];
	// 文字列を取得     
	DWORD result = GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buffer, sizeof(buffer), lpFileName);
	// 文字列が取得できたらfloatに変換して値を返す     
	if (result != 0) return static_cast<float>(atof(buffer));
	else return static_cast<float>(atof(lpDefault));
}