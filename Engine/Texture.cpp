#include <vector>
#include "Texture.h"
#include "Direct3D.h"
#include "Global.h"
#include "Debug.h"

Texture::Texture():
	pSampleLinear_(nullptr), pTextureSRV_(nullptr), size_(XMFLOAT3(0,0,0))
{
}


Texture::~Texture()
{
	SAFE_RELEASE(pSampleLinear_);
	SAFE_RELEASE(pTextureSRV_);
}

HRESULT Texture::Load(std::string fileName)
{
	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fileName.c_str(), fileName.length());


	// テクスチャを読み込む
	// 途中で失敗したら、どこで失敗したかを覚えて、それまでに作ったものを解放する
	// （COMの初期化はMainが行う）
	IWICImagingFactory *pFactory = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode* pFrame = NULL;
	IWICFormatConverter* pFormatConverter = NULL;
	ID3D11Texture2D*	pTexture = NULL;			// テクスチャデータ
	const char* step = "";
	HRESULT hr = S_OK;
	do
	{
		step = "画像を読み込む準備(WIC)ができません";
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void **>(&pFactory));
		if (FAILED(hr))	break;

		step = "画像ファイルを開けません";
		hr = pFactory->CreateDecoderFromFilename(wtext, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (FAILED(hr))	break;

		step = "画像を読み取れません";
		hr = pDecoder->GetFrame(0, &pFrame);
		if (FAILED(hr))	break;

		step = "画像の形式を変換できません";
		hr = pFactory->CreateFormatConverter(&pFormatConverter);
		if (FAILED(hr))	break;
		hr = pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr))	break;

		UINT imgWidth;
		UINT imgHeight;
		step = "画像のサイズを取得できません";
		hr = pFormatConverter->GetSize(&imgWidth, &imgHeight);
		if (FAILED(hr))	break;
		size_ = XMFLOAT3((float)imgWidth, (float)imgHeight, 0);

		// テクスチャの設定
		D3D11_TEXTURE2D_DESC texdec;
		texdec.Width = imgWidth;
		texdec.Height = imgHeight;
		texdec.MipLevels = 1;
		texdec.ArraySize = 1;
		texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texdec.SampleDesc.Count = 1;
		texdec.SampleDesc.Quality = 0;
		texdec.Usage = D3D11_USAGE_DYNAMIC;
		texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		texdec.MiscFlags = 0;
		step = "テクスチャを作成できません";
		hr = Direct3D::pDevice_->CreateTexture2D(&texdec, NULL, &pTexture);
		if (FAILED(hr))	break;

		// テクスチャを送る
		D3D11_MAPPED_SUBRESOURCE hMappedres;
		step = "テクスチャにデータを送れません";
		hr = Direct3D::pContext_->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
		if (FAILED(hr))	break;
		hr = pFormatConverter->CopyPixels(NULL, imgWidth * 4, imgWidth * imgHeight * 4, (BYTE*)hMappedres.pData);
		Direct3D::pContext_->Unmap(pTexture, 0);
		if (FAILED(hr))	break;

		// シェーダリソースビューとサンプラーの作成
		hr = CreateView(pTexture, &step);
	} while (false);

	//途中で使ったものを解放（成功したときも、一時的に使ったものはいらない）
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pFormatConverter);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFactory);

	if (FAILED(hr))
	{
		//失敗したので、作りかけのものも解放して、理由を覚えておく
		SAFE_RELEASE(pSampleLinear_);
		SAFE_RELEASE(pTextureSRV_);
		errorMessage_ = std::string(step) + " " + Debug::HResult(hr);
		return hr;
	}

	return S_OK;
}

//単色のテクスチャを作る
HRESULT Texture::CreateSolid(UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a)
{
	size_ = XMFLOAT3((float)width, (float)height, 0);

	//全ピクセルを同じ色で埋める
	std::vector<BYTE> pixels((size_t)width * height * 4);
	for (size_t i = 0; i < pixels.size(); i += 4)
	{
		pixels[i + 0] = r;
		pixels[i + 1] = g;
		pixels[i + 2] = b;
		pixels[i + 3] = a;
	}

	// テクスチャの設定
	D3D11_TEXTURE2D_DESC texdec = {};
	texdec.Width = width;
	texdec.Height = height;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DEFAULT;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = pixels.data();
	data.SysMemPitch = width * 4;

	ID3D11Texture2D* pTexture = NULL;
	const char* step = "代替のテクスチャを作成できません";
	HRESULT hr = Direct3D::pDevice_->CreateTexture2D(&texdec, &data, &pTexture);
	if (SUCCEEDED(hr))
	{
		hr = CreateView(pTexture, &step);
	}
	SAFE_RELEASE(pTexture);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pSampleLinear_);
		SAFE_RELEASE(pTextureSRV_);
		errorMessage_ = std::string(step) + " " + Debug::HResult(hr);
		return hr;
	}

	return S_OK;
}

//テクスチャからシェーダーリソースビューとサンプラーを作る
HRESULT Texture::CreateView(ID3D11Texture2D* pTexture, const char** ppStep)
{
	// シェーダリソースビュー(テクスチャ用)の設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	*ppStep = "シェーダーリソースビューを作成できません";
	HRESULT hr = Direct3D::pDevice_->CreateShaderResourceView(pTexture, &srv, &pTextureSRV_);
	if (FAILED(hr))	return hr;


	// テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	*ppStep = "サンプラーを作成できません";
	return Direct3D::pDevice_->CreateSamplerState(&SamDesc, &pSampleLinear_);
}