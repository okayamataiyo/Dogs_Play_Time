//インクルード
#include "AIPlayerAttackSeeAction.h"
#include "../Player/AIPlayer.h"

using enum Node::NODESTATE;

AIPlayerAttackSeeAction::AIPlayerAttackSeeAction(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
}

AIPlayerAttackSeeAction::~AIPlayerAttackSeeAction()
{
}

void AIPlayerAttackSeeAction::ChoiceUpdate()
{
	switch (nodeData_.myNodeState_)
	{
	case READY:
		ReadyUpdate();
		break;
	case RUNNING:
		RunningUpdate();
		break;
	case SUCCESS:
		SuccessUpdate();
		break;
	case FAILURE:
		FailureUpdate();
		break;
	}
}

void AIPlayerAttackSeeAction::ReadyUpdate()
{
}

void AIPlayerAttackSeeAction::RunningUpdate()
{
	if (pAIPlayer_->GetIsAttackSee())
	{
		pAIPlayer_->PlayerAttackSeeActionFunc();
	}
	if (pAIPlayer_->GetIsAttackSeeFinish())
	{
		nodeData_.myNodeState_ = SUCCESS;
	}
}

void AIPlayerAttackSeeAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerAttackSeeAction::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}