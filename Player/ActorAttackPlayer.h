#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class Dogs_Walk_PlayScene;
class Stage;

namespace
{
	std::string actorAttackPlayerName = "AttackPlayer";
}

class ActorAttackPlayer : public PlayerBase
{
private:
	int hModel_;
	int stageHModel_;
	bool isSelect_;		//セレクトシーンで呼ばれているかどうか
	GAMESTATE gameState_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	Stage* pStage_;
public:

	ActorAttackPlayer(GameObject* _pParent);

	~ActorAttackPlayer();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;

	void UpdateReady() override;
	void UpdatePlay() override;
	void UpdateGameOver() override;

	void OnCollision(GameObject* _pTarget)override;

	void PlayerScore() override;

	void PlayerCamera() override;

	void PlayerFall() override;

	void PlayerMove() override;

	void PlayerJump() override;

	void PlayerJumpPower() override;

	void PlayerDive() override;

	void PlayerDivePower() override;

	void PlayerKnockback() override;

	void PlayerRayCast() override;

	void PlayerRevival() override;

	void PlayerStun(int _timeLimit = 60) override;

	void IsMove() override;

	void IsJump() override;

	void IsRun() override;

	void IsStun() override;

	void IsDive() override;

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	void SetIsSelect(bool _isSelect) { isSelect_ = _isSelect; }

	bool GetIsMove() override { return moveData_.isMove_; }

	bool GetIsJump() override { return jumpData_.isJump_; }

	bool GetIsRun() override { return moveData_.isRun_; }

	bool GetIsStun() override { return stunData_.isStun_; }

	bool GetIsDive() override { return diveData_.isDive_; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	int GetPadID()override { return gameData_.padID_; }

	int GetModelHandle() override { return hModel_; };
};

