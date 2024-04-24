#pragma once
//インクルード
#include "StateBase.h"

class CollectPlayer;
class AttackPlayer;

class PlayerWaitState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	PlayerWaitState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class PlayerWalkState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	PlayerWalkState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class PlayerRunState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	PlayerRunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class PlayerJumpState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	PlayerJumpState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class PlayerStunState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	PlayerStunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};
