#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class PlayScene;
class CollectPlayer;
class SphereCollider;
class WoodBox;
class Text;
class Stage;
class Floor;
class SceneManager;
class ItemObjectManager;

namespace
{
	std::string soundAttackPlayerNames[] =
	{
		"Stun",
		"Walk",
		"Jump",
		"Run",
	};

	std::string attackPlayerName = "AttackPlayer";
	int attackPlayerNumber = 1;
}

/// <summary>
/// 邪魔するプレイヤーを管理するクラス
/// </summary>
class AttackPlayer : public PlayerBase
{
private:

	enum class SOUNDSTATE
	{
		STUN = 0,
		WALK,
		JUMP,
		RUN,
	};

	//▼ハンドルモデル番号
	int hModel_;					//モデル番号
	int hSound_[4];					//サウンド番号
	int stageHModel_;				//ステージモデル番号
	int floorHModel_;				//すり抜け床モデル番号
	int number_;
	int scoreTimeCounter_;
	int scoreTimeCounterWait_;
	XMVECTOR vecKnockbackDirection_;
	PLAYERSTATE playerState_;
	PLAYERSTATE playerStatePrev_;
	GAMESTATE gameState_;
	GameObject* pParent_;
	PlayScene* pPlayScene_;
	CollectPlayer* pCollectPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	Text* pText_;
	Stage* pStage_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
public:

	/// <summary>
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	AttackPlayer(GameObject* _pParent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~AttackPlayer();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();
	/// <summary>
	/// プレイヤーをスタン(行動不能)にする処理
	/// </summary>
	/// <param name="_timeLimit">_timeLimit秒まで、動かせない</param>
	void Stun(int _timeLimit = 60) override;

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 開放関数
	/// </summary>
	void Release() override;

	/// <summary>
	/// 何かに当たった時の関数
	/// </summary>
	/// <param name="_pTarget">当たった相手</param>
	void OnCollision(GameObject* _pTarget) override;

	void PlayerFall() override;

	/// <summary>
	/// プレイヤーの移動関数
	/// </summary>
	void PlayerMove() override;

	void PlayerJump() override;

	/// <summary>
	/// プレイヤーのジャンプ関数
	/// </summary>
	void PlayerJumpPower() override;

	void PlayerDive() override;

	void PlayerDivePower() override;

	void PlayerKnockback() override;

	/// <summary>
	/// プレイヤーのレイ関数
	/// </summary>
	void PlayerRayCast() override;

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	void SetCollectPlayer(CollectPlayer* _pCollectPlayer) { pCollectPlayer_ = _pCollectPlayer; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	PLAYERSTATE GetPlayerState() override { return playerState_; }

	float GetAngle() { return angle_; }

	int GetPadID()override { return padID_; }

	int GetScore() { return score_; }

	int GetModelHandle() override { return hModel_; };

	int GetIsJump() { return isJump_; }

	int GetIsStun() { return isStun_; }

	bool IsMoving() override;
};

