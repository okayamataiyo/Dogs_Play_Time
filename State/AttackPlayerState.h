#pragma once
//インクルード
#include "StateBase.h"

class CollectPlayer;
class AttackPlayer;

class AttackPlayerWaitState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	AttackPlayerWaitState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AttackPlayerWalkState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	AttackPlayerWalkState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AttackPlayerRunState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	AttackPlayerRunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AttackPlayerJumpState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	AttackPlayerJumpState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AttackPlayerStunState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	AttackPlayerStunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};
