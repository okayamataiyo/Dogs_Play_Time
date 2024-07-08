//インクルード
#include "AIPlayerWaitSelector.h"
#include "AIPlayerWaitAction.h"
#include "AIPlayerAttackDecorator.h"
#include "../Engine/Global.h"

AIPlayerWaitSelector::AIPlayerWaitSelector(Node* _pParentNode,GameObject* _pGameObject)
	:Node(_pParentNode,_pGameObject)
	,pAIPlayerWaitAction_{nullptr},pAIPlayerAttackDecorator_{nullptr}
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerWaitAction_ = new AIPlayerWaitAction(this, nodeData_.pGameObject_);
	pAIPlayerAttackDecorator_ = new AIPlayerAttackDecorator(this, nodeData_.pGameObject_);
}

AIPlayerWaitSelector::~AIPlayerWaitSelector()
{
	SAFE_DELETE(pAIPlayerWaitAction_);
	SAFE_DELETE(pAIPlayerAttackDecorator_);
}

void AIPlayerWaitSelector::ChoiceUpdate()
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

void AIPlayerWaitSelector::ReadyUpdate()
{
}

void AIPlayerWaitSelector::RunningUpdate()
{
	if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::READY)
	{
		pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::RUNNING);
		pAIPlayerAttackDecorator_->ChoiceUpdate();
	}
	if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::SUCCESS)
	{
		pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::READY);
		nodeData_.myNodeState_ = NODESTATE::SUCCESS;
	}
	if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::FAILURE)
	{
		pAIPlayerWaitAction_->ChoiceUpdate();
		pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::READY);
		nodeData_.myNodeState_ = NODESTATE::FAILURE;
	}
	if (pAIPlayerWaitAction_->GetMyNodeState() == NODESTATE::SUCCESS)
	{
		pAIPlayerWaitAction_->SetMyNodeState(NODESTATE::READY);
		nodeData_.myNodeState_ = NODESTATE::SUCCESS;
	}
}

void AIPlayerWaitSelector::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerWaitSelector::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}
