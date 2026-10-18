#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wincodec.h>
#include <wrl/client.h>
#include "string"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")
#pragma comment( lib, "WindowsCodecs.lib" )

using namespace DirectX;

//-----------------------------------------------------------
//テクスチャ関連を扱うクラス
//-----------------------------------------------------------
class Texture
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSampleLinear_;		//テクスチャサンプラー（テクスチャの貼り方）
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pTextureSRV_;		//シェーダーリソースビュー（テクスチャをシェーダーに送るためのもの）
	XMFLOAT3					size_;				//画像ファイルのサイズ
	std::string					errorMessage_;		//読み込めなかったときの理由

	//テクスチャからシェーダーリソースビューとサンプラーを作る
	//引数：pTexture	元になるテクスチャ
	//引数：ppStep		失敗したときに、どの手順で失敗したかの説明を受け取る
	HRESULT CreateView(ID3D11Texture2D* pTexture, const char** ppStep);

public:
	Texture();

	//ロード
	//引数：fileName	画像ファイル名
	//戻値：成功/失敗（失敗した理由はGetErrorMessageで取得できる）
	HRESULT Load(std::string fileName);

	//単色のテクスチャを作る（画像が読み込めなかったときの代替に使う）
	//引数：width, height	テクスチャの大きさ
	//引数：r, g, b, a		色（0?255）
	//戻値：成功/失敗
	HRESULT CreateSolid(UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a);

	//読み込めなかったときの理由を取得
	//戻値：理由（Load / CreateSolidが失敗したときだけ入っている）
	std::string GetErrorMessage() { return errorMessage_; }


	//各アクセス関数
	ID3D11SamplerState* GetSampler() { return pSampleLinear_.Get(); }	//サンプラーの取得
	ID3D11ShaderResourceView* GetSRV() { return pTextureSRV_.Get(); }	//シェーダーリソースビューの取得
	XMFLOAT3 GetSize() { return size_; }	//画像サイズの取得
};

