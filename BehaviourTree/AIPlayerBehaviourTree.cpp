//インクルード
#include "../Player/AIPlayer.h"
#include "BehaviourTreeManager.h"
#include "AIPlayerBehaviourTree.h"

AIPlayerWaitSelectorTree::AIPlayerWaitSelectorTree(BehaviourTreeManager* _pBehaviourTreeManager)
	:BehaviourTreeBase(_pBehaviourTreeManager), pAIPlayer_{nullptr}, treeState_{TREE::READY}
{
	pAIPlayer_ = (AIPlayer*)(pBehaviourTreeManager_->GetGameObject());
}

void AIPlayerWaitSelectorTree::EnterState()
{
	treeState_ = TREE::RUNNING;
}

void AIPlayerWaitSelectorTree::UpdateState()
{
	pBehaviourTreeManager_->ChangeState("AttackDecoratorTree");
}

void AIPlayerWaitSelectorTree::ExitState()
{

}

AIPlayerAttackDecoratorTree::AIPlayerAttackDecoratorTree(BehaviourTreeManager* _pBehaviourTreeManager)
	:BehaviourTreeBase(_pBehaviourTreeManager), pAIPlayer_{nullptr}, treeState_{TREE::READY}
{
	pAIPlayer_ = (AIPlayer*)(pBehaviourTreeManager_->GetGameObject());
}

void AIPlayerAttackDecoratorTree::EnterState()
{
	treeState_ = TREE::RUNNING;
}

void AIPlayerAttackDecoratorTree::UpdateState()
{
	pAIPlayer_->PlayerAttackDecoratorTreeFunc();
	if (pAIPlayer_->GetIsWaitSelector())
	{
		pBehaviourTreeManager_->ChangeState("ActionActionTree");
	}
}

void AIPlayerAttackDecoratorTree::ExitState()
{

}

AIPlayerAttackActionTree::AIPlayerAttackActionTree(BehaviourTreeManager* _pBehaviourTreeManager)
	:BehaviourTreeBase(_pBehaviourTreeManager), pAIPlayer_{nullptr}, treeState_{TREE::READY}
{
	pAIPlayer_ = (AIPlayer*)(pBehaviourTreeManager_->GetGameObject());
}

void AIPlayerAttackActionTree::EnterState()
{
	treeState_ = TREE::RUNNING;
}

void AIPlayerAttackActionTree::UpdateState()
{
	pAIPlayer_->PlayerAttackActionTreeFunc();
	if (!pAIPlayer_->GetIsWaitSelector())
	{
		pBehaviourTreeManager_->ChangeState("WaitActionTree");
	}
}

void AIPlayerAttackActionTree::ExitState()
{
}
