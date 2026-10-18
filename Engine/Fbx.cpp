#include "Fbx.h"
#include "Direct3D.h"
#include "FbxParts.h"
#include "Debug.h"



Fbx::Fbx():_animSpeed(0)
{
}


//メンバーが宣言の逆順（パーツ → シーン → FBX SDK本体）に解放される
//FbxPartsが不完全な型のヘッダーでは定義できないので、ここで定義する
Fbx::~Fbx() = default;

HRESULT Fbx::Load(std::string fileName)
{
	// FBXの読み込み
	pFbxManager_.reset(FbxManager::Create());
	pFbxScene_.reset(FbxScene::Create(pFbxManager_.get(), "fbxscene"));

	//ファイルが無いときは、FBX SDKのエラー文言ではわかりにくいので、先に調べる
	if (GetFileAttributes(fileName.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		errorMessage_ = "ファイルが見つかりません";
		return E_FAIL;
	}
	FbxString FileName(fileName.c_str());
	{
		//読み込み用のインポーター（このブロックを抜けると、Destroy()で解放される）
		FbxUniquePtr<FbxImporter> fbxImporter(FbxImporter::Create(pFbxManager_.get(), "imp"));
		if (!fbxImporter->Initialize(FileName.Buffer(), -1, pFbxManager_->GetIOSettings()))
		{
			//失敗
			errorMessage_ = fbxImporter->GetStatus().GetErrorString();
			return E_FAIL;
		}
		if (!fbxImporter->Import(pFbxScene_.get()))
		{
			//失敗
			errorMessage_ = fbxImporter->GetStatus().GetErrorString();
			return E_FAIL;
		}
	}

	// アニメーションのタイムモードの取得
	_frameRate = pFbxScene_->GetGlobalSettings().GetTimeMode();


	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//カレントディレクトリをファイルがあった場所に変更
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);
	SetCurrentDirectory(dir);



	//ルートノードを取得して
	FbxNode* rootNode = pFbxScene_->GetRootNode();

	//そいつの子供の数を調べて
	int childCount = rootNode->GetChildCount();

	//1個ずつチェック
	HRESULT hr = S_OK;
	for (int i = 0; childCount > i; i++)
	{
		hr = CheckNode(rootNode->GetChild(i), &parts_);
		if (FAILED(hr))
		{
			break;
		}
	}



	//カレントディレクトリを元の位置に戻す
	SetCurrentDirectory(defaultCurrentDir);

	return hr;
}

HRESULT Fbx::CheckNode(FbxNode * pNode, std::vector<std::unique_ptr<FbxParts>>* pPartsList)
{
	//そのノードにはメッシュ情報が入っているだろうか？
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr != nullptr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//パーツを用意
		std::unique_ptr<FbxParts> pParts = std::make_unique<FbxParts>();
		HRESULT hr = pParts->Init(pNode);

		//パーツ情報を動的配列に追加（失敗したときも、解放はデストラクタに任せる）
		pPartsList->push_back(std::move(pParts));

		if (FAILED(hr))
		{
			errorMessage_ = "メッシュを準備できません。" + Debug::HResult(hr);
			return hr;
		}
	}


	//子ノードにもデータがあるかも！！
	{
		//子供の数を調べて
		int childCount = pNode->GetChildCount();

		//一人ずつチェック
		for (int i = 0; i < childCount; i++)
		{
			HRESULT hr = CheckNode(pNode->GetChild(i), pPartsList);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}

	return S_OK;
}

XMFLOAT3 Fbx::GetBonePosition(std::string boneName)
{
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePosition(boneName, &position))
			break;
	}
	return position;
}

void Fbx::Draw(Transform& transform, int frame)
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);

	//パーツを1個ずつ描画
	for (int k = 0; k < parts_.size(); k++)
	{
		// その瞬間の自分の姿勢行列を得る
		FbxTime     time;
		time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);


		//スキンアニメーション（ボーン有り）の場合
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawSkinAnime(transform, time);
		}

		//メッシュアニメーションの場合
		else
		{
			parts_[k]->DrawMeshAnime(transform, time, pFbxScene_.get());
		}
	}
}


//レイキャスト（レイを飛ばして当たり判定）
void Fbx::RayCast(RayCastData * data)
{
	//すべてのパーツと判定
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(data);
	}
}
