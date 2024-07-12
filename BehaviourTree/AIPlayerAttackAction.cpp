//インクルード
#include "AIPlayerAttackAction.h"
#include "../Player/AIPlayer.h"

AIPlayerAttackAction::AIPlayerAttackAction(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
}

AIPlayerAttackAction::~AIPlayerAttackAction()
{
}

void AIPlayerAttackAction::ChoiceUpdate()
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

void AIPlayerAttackAction::ReadyUpdate()
{
}

void AIPlayerAttackAction::RunningUpdate()
{
	pAIPlayer_->PlayerAttackActionFunc();
	if (!pAIPlayer_->GetIsAttack())
	{
		nodeData_.myNodeState_ = NODESTATE::SUCCESS;
	}
}

void AIPlayerAttackAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerAttackAction::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}