#pragma once
//インクルード
#include "BehaviourTreeBase.h"

class AIPlayer;

class AIPlayerWaitActionTree : public BehaviourTreeBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerWaitActionTree(BehaviourTreeManager* _pBehaviourTreeManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};


class AIPlayerAttackActionTree : public BehaviourTreeBase
{
private:
	AIPlayer* pAIPlayer_;
public:
	AIPlayerAttackActionTree(BehaviourTreeManager* _pBehaviourTreeManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

