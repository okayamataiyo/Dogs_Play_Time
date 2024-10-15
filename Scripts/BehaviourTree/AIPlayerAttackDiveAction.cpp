//インクルード
#include "AIPlayerAttackDiveAction.h"
#include "../Player/AIPlayer.h"

using enum Node::NODESTATE;

AIPlayerAttackDiveAction::AIPlayerAttackDiveAction(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = READY;
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

void AIPlayerAttackDiveAction::ReadyUpdate()
{
}

void AIPlayerAttackDiveAction::RunningUpdate()
{
	pAIPlayer_->PlayerAttackDiveActionFunc();
	nodeData_.myNodeState_ = SUCCESS;
}

void AIPlayerAttackDiveAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerAttackDiveAction::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}