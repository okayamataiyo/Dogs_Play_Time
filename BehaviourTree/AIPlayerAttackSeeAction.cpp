//インクルード
#include "AIPlayerAttackSeeAction.h"
#include "../Player/AIPlayer.h"

AIPlayerAttackSeeAction::AIPlayerAttackSeeAction(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = NODESTATE::READY;
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
	case NODESTATE::READY:
		ReadyUpdate();
		break;
	case NODESTATE::RUNNING:
		RunningUpdate();
		break;
	case NODESTATE::SUCCESS:
		SuccessUpdate();
		break;
	case NODESTATE::FAILURE:
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
		nodeData_.myNodeState_ = NODESTATE::SUCCESS;
	}
}

void AIPlayerAttackSeeAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerAttackSeeAction::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}