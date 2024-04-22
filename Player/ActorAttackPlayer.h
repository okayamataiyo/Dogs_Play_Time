#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class PlayScene;
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
	PLAYERSTATE playerState_;
	PLAYERSTATE playerStatePrev_;
	GAMESTATE gameState_;
	PlayScene* pPlayScene_;
	Stage* pStage_;
public:

	ActorAttackPlayer(GameObject* _pParent);

	~ActorAttackPlayer();

	void Initialize() override;

	void Update() override;

	void Stun(int _timeLimit = 60) override;

	void Draw() override;

	void Release() override;

	void UpdateReady() override;
	void UpdatePlay() override;
	void UpdateGameOver() override;

	void OnCollision(GameObject* _pTarget)override;

	void PlayerFall() override;

	void PlayerMove() override;

	void PlayerJump() override;

	void PlayerJumpPower() override;

	void PlayerDive() override;

	void PlayerDivePower() override;

	void PlayerKnockback() override;

	void PlayerRayCast() override;

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	PLAYERSTATE GetPlayerState() override { return playerState_; }

	int GetPadID()override { return padID_; }

	int GetModelHandle() override { return hModel_; };

	bool IsMoving() override;
};

