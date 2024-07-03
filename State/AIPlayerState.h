#pragma once
//�C���N���[�h
#include "StateBase.h"

class AIPlayer;

enum class TREE
{
	READY = 0,	//�����l
	SUCCESS,	//�m�[�h�̎��s����������
	FAILURE,	//�m�[�h�̎��s�����s����
	RUNNING,	//�m�[�h�̎��s���܂��I����Ă��Ȃ�

};

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

class AIPlayerAttackDirActionState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerAttackDirActionState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AIPlayerAttackGoActionState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerAttackGoActionState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};



class AIPlayerSelectorState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerSelectorState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

class AIPlayerSequenceState : public StateBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerSequenceState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

