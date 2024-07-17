//インクルード
#include "AIPlayerAttackDiveAction.h"
#include "../Player/AIPlayer.h"

AIPlayerAttackDiveAction::AIPlayerAttackDiveAction(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
}

AIPlayerAttackDiveAction::~AIPlayerAttackDiveAction()
{
}

void AIPlayerAttackDiveAction::ChoiceUpdate()
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

void AIPlayerAttackDiveAction::ReadyUpdate()
{
}

void AIPlayerAttackDiveAction::RunningUpdate()
{
	pAIPlayer_->PlayerAttackDiveActionFunc();
	nodeData_.myNodeState_ = NODESTATE::SUCCESS;
}

void AIPlayerAttackDiveAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerAttackDiveAction::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}