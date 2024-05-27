#pragma once
//インクルード
#include "ItemObjectBase.h"

class Dogs_Walk_PlayScene;
class SphereCollider;
class Stage;
class CollectPlayer;
class Bone;

namespace
{
	std::string boneSuckName = "Bone";
}

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class BoneSuck : public ItemObjectBase
{
protected:
	GameObject* pParent_;
	int hModel_;
	int playerHModel_;
	float rayDist_;
	float positionRotate_;
	float BoneSuckInitPosY_;
	int killTime_;
	int killTimeWait_;
	int killTimeMax_;
	XMFLOAT3 pickUpBoneSuckScale_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	SphereCollider* pCollision_;
	Stage* pStage_;
	CollectPlayer* pCollectPlayer_;
	Bone* pBone_;
public:
	/// <summary>
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	BoneSuck(GameObject* _parent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~BoneSuck();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 開放関数
	/// </summary>
	void Release() override;

	void PlayerSuckBoneSuck();

	void BoneSuckDeath();

	void SetKillTime(int _killTime) { killTime_ = _killTime; }

	int GetModelHandle() { return hModel_; }


};