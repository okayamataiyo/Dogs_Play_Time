//インクルード
#include "../Player/AIPlayer.h"
#include "BehaviourTreeManager.h"
#include "AIPlayerBehaviourTree.h"

AIPlayerWaitActionTree::AIPlayerWaitActionTree(BehaviourTreeManager* _pBehaviourTreeManager)
	:BehaviourTreeBase(_pBehaviourTreeManager), pAIPlayer_{ nullptr }
{
	pAIPlayer_ = (AIPlayer*)(pBehaviourTreeManager_->GetGameObject());
}

void AIPlayerWaitActionTree::EnterState()
{

}

void AIPlayerWaitActionTree::UpdateState()
{
	pAIPlayer_->PlayerWaitActionTreeFunc();
	if (pAIPlayer_->GetIsWaitSelector())
	{
		pBehaviourTreeManager_->ChangeState("AttackActionTree");
	}
}

void AIPlayerWaitActionTree::ExitState()
{
}

AIPlayerAttackActionTree::AIPlayerAttackActionTree(BehaviourTreeManager* _pBehaviourTreeManager)
	:BehaviourTreeBase(_pBehaviourTreeManager),pAIPlayer_{nullptr}
{
	pAIPlayer_ = (AIPlayer*)(pBehaviourTreeManager_->GetGameObject());
}

void AIPlayerAttackActionTree::EnterState()
{
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
