#pragma once
//インクルード
#include "../../Engine/GameObject.h"
#include "PlayerBase.h"

class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;
class AttackPlayer;
class AIPlayer;
class SphereCollider;
class WoodBox;
class BoneSuck;
class Stage;
class StageBlock;
class Floor;
class SceneManager;
class ItemObjectManager;
class StateManager;
class ImageManager;
class ParticleManager;
class UIManager;

namespace
{
	const std::string soundCollectPlayerNames[] =
	{
		"Stun",
		"Walk",
		"Jump",
		"Run",
		"CollectBone",
	};

	const std::string collectPlayerName = "CollectPlayer";


}

/// <summary>
/// 収集するプレイヤーを管理するクラス
/// </summary>
class CollectPlayer : public PlayerBase
{
public:
	enum class SOUNDSTATE
	{
		STUN = 0,
		WALK,
		JUMP,
		RUN,
		CollectBone,
	};
private:

	//▼ハンドルモデル番号
	int hModel_;					//モデル番号
	int hSound_[5];					//サウンド番号
	int stageBlockHModel_;			//ステージブロックモデル番号
	int stageHModel_;				//ステージモデル番号
	int floorHModel_;				//すり抜け床
	int number_;
	int attackOrCollectInverse_;
	const float pi_ = 3.14f;					//円周率
	const float halfPi_ = pi_ / 2;				//円周率の半分

	GAMESTATE gameState_;
	GameObject* pParent_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
	AttackPlayer* pAttackPlayer_;
	AIPlayer* pAIPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	BoneSuck* pBoneSuck_;
	Stage* pStage_;
	StageBlock* pStageBlock_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
	StateManager* pStateManager_;
	ImageManager* pImageManager_;
	ImageManager* pBoneImageManager_;
	ParticleManager* pParticleManager_;
public:

	/// <summary>
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	CollectPlayer(GameObject* _pParent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~CollectPlayer();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

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
	/// プレイヤーの移動関数
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
	/// プレイヤーをスタン(行動不能)にする処理
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
	void SetAttackPlayer(AttackPlayer* _pAttackPlayer) { pAttackPlayer_ = _pAttackPlayer; }
	void SetAIPlayer(AIPlayer* _pAIPlayer) { pAIPlayer_ = _pAIPlayer; }
	void SetKillTime(int _killTime) { boneData_.killTime_ = _killTime; }
	void SetImageSecInit();

	//▼ゲッター
	bool GetIsMove() override { return moveData_.isMove_; }
	bool GetIsJump() override { return jumpData_.isJump_; }
	bool GetIsRun() override { return moveData_.isRun_; }
	bool GetIsStun() override { return stunData_.isStun_; }
	bool GetIsDive() override { return diveData_.isDive_; }
	bool GetIsBoneTatch() {return boneData_.isBoneTatch_; }
	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }
	int GetPadID()override { return gameData_.padID_; }
	int GetModelHandle() override { return hModel_; }
	float GetAngle() { return dirData_.angle_; }
	int GetScore() { return gameData_.score_; }
};

