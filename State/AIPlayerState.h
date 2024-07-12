#pragma once
//インクルード
#include "StateBase.h"

class AIPlayer;

class AIPlayerWaitState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerWaitState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AIPlayerWalkState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerWalkState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AIPlayerRunState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerRunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AIPlayerJumpState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerJumpState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AIPlayerStunState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerStunState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};