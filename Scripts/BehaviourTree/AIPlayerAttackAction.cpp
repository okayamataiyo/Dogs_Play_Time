//インクルード
#include "AIPlayerAttackAction.h"
#include "../Player/AIPlayer.h"

using enum NODESTATE;

AIPlayerAttackAction::AIPlayerAttackAction(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = READY;
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

void AIPlayerAttackAction::ReadyUpdate()
{
}

void AIPlayerAttackAction::RunningUpdate()
{
	pAIPlayer_->PlayerAttackActionFunc();
	if (!pAIPlayer_->GetIsAttack())
	{
		nodeData_.myNodeState_ = SUCCESS;
	}
}

void AIPlayerAttackAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerAttackAction::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}