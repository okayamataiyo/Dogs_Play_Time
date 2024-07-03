#pragma once
//インクルード
#include "BehaviourTreeBase.h"

class AIPlayer;

class AIPlayerWaitActionTree : public BehaviourTreeBase
{
private:
	AIPlayer* pAIPlayer_;
	TREE treeState_;
public:
	AIPlayerWaitActionTree(BehaviourTreeManager* _pBehaviourTreeManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
	TREE GetTreeState() { return treeState_; }
};


class AIPlayerAttackActionTree : public BehaviourTreeBase
{
private:
	AIPlayer* pAIPlayer_;
	TREE treeState_;
public:
	AIPlayerAttackActionTree(BehaviourTreeManager* _pBehaviourTreeManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
	TREE GetTreeState() { return treeState_; }
};

