#pragma once
//インクルード
#include "StateBase.h"

class CollectPlayer;
class AttackPlayer;

class CollectPlayerWaitState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	CollectPlayerWaitState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class CollectPlayerWalkState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	CollectPlayerWalkState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class CollectPlayerRunState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	CollectPlayerRunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class CollectPlayerJumpState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	CollectPlayerJumpState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class CollectPlayerStunState : public StateBase
{
private:
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	CollectPlayerStunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};
