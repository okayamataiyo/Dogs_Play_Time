#pragma once
//インクルード
#include "../../Engine/GameObject.h"
#include "PlayerBase.h"

class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;
class CollectPlayer;
class AttackPlayer;
class SphereCollider;
class WoodBox;
class Text;
class Stage;
class Floor;
class SceneManager;
class ItemObjectManager;
class StateManager;
class ImageManager;
class ParticleManager;
class UIManager;
class AIPlayerWaitSelector;

namespace
{
	std::string soundAIPlayerNames[] =
	{
		"Stun",
		"Walk",
		"Jump",
		"Run",
	};

	std::string aIPlayerName = "AIPlayer";
}

/// <summary>
/// 邪魔するAIプレイヤーを管理するクラス
/// </summary>
class AIPlayer : public PlayerBase
{
private:

	//▼ハンドルモデル番号
	int hModel_;					//モデル番号
	int hSound_[5];					//サウンド番号
	int stageHModel_;				//ステージモデル番号
	int floorHModel_;				//すり抜け床モデル番号
	int number_;
	int slowTime_;
	int slowTimeWait_;
	const int slowTimeNum_ = 2;
	const int defaultTimeNum_ = 1;
	int waitTime_;
	int waitTimeWait_;
	int attackTime_;
	int attackTimeWait_;
	bool isAttack_;
	bool isAttackFinish_;
	int attackSeeTime_;
	int attackSeeTimeWait_;
	bool isAttackSee_;
	bool isAttackSeeFinish_;
	int random_value_;
	const int revivalTime_ = 60;
	XMVECTOR dir_;

	GAMESTATE gameState_;
	GameObject* pParent_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	Stage* pStage_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
	StateManager* pStateManager_;
	ImageManager* pImageManager_;
	ImageManager* pBoneImageManager_;
	ParticleManager* pParticleManager_;
	AIPlayerWaitSelector* pAIPlayerWaitSelector_;
public:
	enum class SOUNDSTATE
	{
		STUN = 0,
		WALK,
		JUMP,
		RUN,
		COLLECTBONE,
	};

	/// <summary>
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	AIPlayer(GameObject* _pParent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~AIPlayer();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// スローモーションに使用しようとした(未完成)
	/// </summary>
	void UpdateSlow();

	//▼描画関数
	//左右画面
	//左画面
	//右画面
	//上画面
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;

	/// <summary>
	/// 開放関数
	/// </summary>
	void Release() override;

	//▼更新関数
	//準備状態
	//プレイ状態
	//ゲームオーバー状態
	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();

	//▼プレイヤーの行動を制御する関数
	void PlayerAttackActionFunc();
	void PlayerAttackSeeActionFunc();
	void PlayerAttackDiveActionFunc();
	void PlayerWaitStateFunc();
	void PlayerWalkStateFunc();
	void PlayerRunStateFunc();
	void PlayerJumpStateFunc();
	void PlayerStunStateFunc();

	/// <summary>
	/// 何かに当たった時の関数
	/// </summary>
	/// <param name="_pTarget">当たった相手</param>
	void OnCollision(GameObject* _pTarget) override;

	/// <summary>
	/// スコア追加関数
	/// </summary>
	void PlayerAddScore() override;

	/// <summary>
	/// カメラを制御する関数
	/// </summary>
	void PlayerCamera() override;

	/// <summary>
	/// 落下した際の処理を制御する関数
	/// </summary>
	void PlayerFall() override;

	/// <summary>
	/// プレイヤーの移動を制御する関数
	/// </summary>
	void PlayerMove() override;

	/// <summary>
	/// プレイヤーのジャンプを制御する関数
	/// </summary>
	void PlayerJump() override;

	/// <summary>
	/// プレイヤーのジャンプの力を加える関数
	/// </summary>
	void PlayerJumpPower() override;

	/// <summary>
	/// プレイヤーの飛びつきを制御する関数
	/// </summary>
	void PlayerDive() override;

	/// <summary>
	/// プレイヤーの飛びつきの力を加える関数
	/// </summary>
	void PlayerDivePower() override;

	/// <summary>
	/// プレイヤーのノックバックを制御する関数
	/// </summary>
	void PlayerKnockback() override;

	/// <summary>
	/// プレイヤーのレイキャストを制御する関数
	/// </summary>
	void PlayerRayCast() override;

	/// <summary>
	/// プレイヤーの復活を制御する関数
	/// </summary>
	void PlayerRevival() override;

	/// <summary>
	/// プレイヤーをスタン(行動不能)にする関数
	/// </summary>
	/// <param name="_timeLimit">_timeLimit秒まで、動かせない</param>
	void PlayerStun(int _timeLimit = 60) override;

	/// <summary>
	/// プレイヤーに対する透明な壁を制御する関数
	/// </summary>
	void PlayerOuterWall() override;

	//▼フラグを制御する関数
	void IsMove() override;
	void IsJump() override;
	void IsRun() override;
	void IsStun() override;
	void IsDive() override;

	//▼セッター
	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }
	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;
	void SetCollectPlayer(CollectPlayer* _pCollectPlayer) { pCollectPlayer_ = _pCollectPlayer; }
	void SetAttackPlayer(AttackPlayer* _pAttackPlayer) { pAttackPlayer_ = _pAttackPlayer; }
	void SetKillTime(int _killTime) { boneData_.killTime_ = _killTime; }
	void SetImageSecInit();

	//▼ゲッター
	bool GetIsMove() override { return moveData_.isMove_; }
	bool GetIsJump() override { return jumpData_.isJump_; }
	bool GetIsRun() override { return moveData_.isRun_; }
	bool GetIsStun() override { return stunData_.isStun_; }
	bool GetIsDive() override { return diveData_.isDive_; }
	bool GetIsAttack() { return isAttack_; }
	bool GetIsAttackSee() { return isAttackSee_; }
	bool GetIsAttackSeeFinish() { return isAttackSeeFinish_; }
	bool GetIsBoneTatch() { return boneData_.isBoneTatch_; }
	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }
	int GetPadID()override { return gameData_.padID_; }
	int GetModelHandle() override { return hModel_; }
	float GetAngle() { return dirData_.angle_; }
	int GetScore() { return gameData_.score_; }
};

