#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <vector>
#include <memory>
#include <string>
#include "Transform.h"
#include "RayCastData.h"

class FbxParts;

//FBX SDKのオブジェクトを解放する削除関数（deleteではなく、Destroy()で解放する決まり）
template<class T>
struct FbxDestroyer
{
	void operator()(T* p) const
	{
		if (p != nullptr)
		{
			p->Destroy();
		}
	}
};

//FBX SDKのオブジェクトを持つunique_ptr
template<class T>
using FbxUniquePtr = std::unique_ptr<T, FbxDestroyer<T>>;

//-----------------------------------------------------------
//　FBXファイルを扱うクラス
//　ほとんどの処理は各パーツごとにFbxPartsクラスで行う
//-----------------------------------------------------------
class Fbx
{
	//FbxPartクラスをフレンドクラスにする
	//FbxPartのprivateな関数にもアクセス可
	friend class FbxParts;



	//FBXファイルを扱う機能の本体
	//（メンバーは宣言の逆順に解放される。パーツ → シーン → 本体の順に解放したいので、この順に宣言する）
	FbxUniquePtr<FbxManager>	pFbxManager_;

	//FBXファイルのシーン（Mayaで作ったすべての物体）を扱う
	FbxUniquePtr<FbxScene>	pFbxScene_;

	//モデルの各パーツ（複数あるかも）
	std::vector<std::unique_ptr<FbxParts>>	parts_;


	// アニメーションのフレームレート
	FbxTime::EMode	_frameRate;

	//アニメーション速度
	float			_animSpeed;

	//アニメーションの最初と最後のフレーム
	int _startFrame, _endFrame;





	//読み込めなかったときの理由
	std::string errorMessage_;

	//ノードの中身を調べる
	//引数：pNode		調べるノード
	//引数：pPartsList	パーツのリスト
	//戻値：成功ならS_OK、失敗ならHRESULT
	HRESULT CheckNode(FbxNode* pNode, std::vector<std::unique_ptr<FbxParts>> *pPartsList);




public:
	Fbx();
	~Fbx();

	//ロード
	//引数：fileName	ファイル名
	//戻値：成功したかどうか
	virtual HRESULT Load(std::string fileName);

	//読み込めなかったときの理由を取得
	//戻値：理由（Load()が失敗したときだけ入っている）
	std::string GetErrorMessage() { return errorMessage_; }

	//描画
	//引数：World	ワールド行列
	void    Draw(Transform& transform, int frame);

	//任意のボーンの位置を取得
	//引数：boneName	取得したいボーンの位置
	//戻値：ボーンの位置
	XMFLOAT3 GetBonePosition(std::string boneName);

	//レイキャスト（レイを飛ばして当たり判定）
	//引数：data	必要なものをまとめたデータ
	void RayCast(RayCastData *data);

};

