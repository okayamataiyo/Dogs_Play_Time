#include "Global.h"
#include "Image.h"
#include "Debug.h"

//3D画像を管理する
namespace Image
{
	//ロード済みの画像データ一覧
	std::vector<std::unique_ptr<ImageData>>	_datas;

	//指定した番号の画像が使えるか（範囲内で、解放されていない）
	static bool IsValid(int handle)
	{
		return handle >= 0 && handle < (int)_datas.size() && _datas[handle] != nullptr;
	}

	//初期化
	void Initialize()
	{
		AllRelease();
	}

	//画像をロード
	int Load(std::string fileName)
	{
		std::unique_ptr<ImageData> pData = std::make_unique<ImageData>();

		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
			{
				pData->pSprite = _datas[i]->pSprite;
				isExist = true;
				break;
			}
		}

		//新たにファイルを開く
		if (isExist == false)
		{
			pData->pSprite = std::make_shared<Sprite>();
			if (FAILED(pData->pSprite->Load(fileName)))
			{
				//開けなかった
				std::string message = "画像を読み込めません。代替の画像を表示します: " + fileName;
				if (!pData->pSprite->GetErrorMessage().empty())
				{
					message += " (" + pData->pSprite->GetErrorMessage() + ")";
				}
				Debug::Warning(message);
				pData->pSprite.reset();

				//代替の画像（目立つマゼンタの矩形）に差し替える
				pData->pSprite = std::make_shared<Sprite>();
				if (FAILED(pData->pSprite->LoadPlaceholder()))
				{
					Debug::Warning("代替の画像も作成できません: " + pData->pSprite->GetErrorMessage());
					pData->pSprite.reset();
					return -1;
				}
			}

			//無事開けた
			pData->fileName = fileName;
		}


		//使ってない番号が無いか探す
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] == nullptr)
			{
				_datas[i] = std::move(pData);

				//切り抜き範囲をリセット
				ResetRect(i);

				return i;
			}
		}

		//新たに追加
		_datas.push_back(std::move(pData));

		//画像番号割り振り
		int handle = (int)_datas.size() - 1;

		//切り抜き範囲をリセット
		ResetRect(handle);

		return handle;
	}



	//描画
	void Draw(int handle)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}
		_datas[handle]->transform.Calclation();
		_datas[handle]->pSprite->Draw(_datas[handle]->transform, _datas[handle]->rect, _datas[handle]->alpha);
	}



	//任意の画像を開放
	void Release(int handle)
	{
		if (!IsValid(handle))
		{
			return;
		}

		//この画像を手放す（同じSpriteを使っている画像が他にいなければ、Spriteもここで消える）
		_datas[handle].reset();
	}



	//全ての画像を解放
	void AllRelease()
	{
		for (int i = 0; i < _datas.size(); i++)
		{
			Release(i);
		}
		_datas.clear();
	}


	//切り抜き範囲の設定
	void SetRect(int handle, int x, int y, int width, int height)
	{
		if (!IsValid(handle))
		{
			return;
		}

		_datas[handle]->rect.left = x;
		_datas[handle]->rect.top = y;
		_datas[handle]->rect.right = width;
		_datas[handle]->rect.bottom = height;
	}


	//切り抜き範囲をリセット（画像全体を表示する）
	void ResetRect(int handle)
	{
		if (!IsValid(handle))
		{
			return;
		}

		XMFLOAT3 size = _datas[handle]->pSprite->GetTextureSize();

		_datas[handle]->rect.left = 0;
		_datas[handle]->rect.top = 0;
		_datas[handle]->rect.right = (long)size.x;
		_datas[handle]->rect.bottom = (long)size.y;

	}

	//アルファ値設定
	void SetAlpha(int handle, int alpha)
	{
		if (!IsValid(handle))
		{
			return;
		}
		_datas[handle]->alpha = (float)alpha / 255.0f;
	}


	//ワールド行列を設定
	void SetTransform(int handle, Transform& transform)
	{
		if (!IsValid(handle))
		{
			return;
		}

		_datas[handle]->transform = transform;
	}


	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle)
	{
		if (!IsValid(handle))
		{
			return XMMatrixIdentity();
		}
		return _datas[handle]->transform.GetWorldMatrix();
	}
	XMFLOAT3 GetSize(int _handle)
	{
		if (!IsValid(_handle))	return XMFLOAT3(0, 0, 0);

		return _datas[_handle]->pSprite->GetTextureSize();
	}
	int GetWidth(int _handle)
	{
		if (!IsValid(_handle))	return 0;

		return _datas[_handle]->pSprite->GetTextureSize().x;
	}
	int GetHeight(int _handle)
	{
		if (!IsValid(_handle))	return 0;

		return _datas[_handle]->pSprite->GetTextureSize().y;
	}
}

