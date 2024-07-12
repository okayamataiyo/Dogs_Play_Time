//インクルード
#include "AIPlayerWaitSelector.h"
#include "AIPlayerWaitAction.h"
#include "AIPlayerAttackSequence.h"
#include "../Engine/Global.h"

AIPlayerWaitSelector::AIPlayerWaitSelector(Node* _pParentNode,GameObject* _pGameObject)
	:Node(_pParentNode,_pGameObject)
	,pAIPlayerWaitAction_{nullptr},pAIPlayerAttackSequence_{nullptr}
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerWaitAction_ = new AIPlayerWaitAction(this, nodeData_.pGameObject_);
	pAIPlayerAttackSequence_ = new AIPlayerAttackSequence(this, nodeData_.pGameObject_);
}

AIPlayerWaitSelector::~AIPlayerWaitSelector()
{
	SAFE_DELETE(pAIPlayerWaitAction_);
	SAFE_DELETE(pAIPlayerAttackSequence_);
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
	if (pAIPlayerAttackSequence_->GetMyNodeState() == NODESTATE::READY)
	{
		pAIPlayerAttackSequence_->SetMyNodeState(NODESTATE::RUNNING);
	}
	if (pAIPlayerAttackSequence_->GetMyNodeState() == NODESTATE::RUNNING)
	{
		pAIPlayerAttackSequence_->ChoiceUpdate();
	}
	if (pAIPlayerAttackSequence_->GetMyNodeState() == NODESTATE::SUCCESS)
	{
		pAIPlayerAttackSequence_->SetMyNodeState(NODESTATE::READY);
		nodeData_.myNodeState_ = NODESTATE::SUCCESS;
	}
	if (pAIPlayerAttackSequence_->GetMyNodeState() == NODESTATE::FAILURE)
	{
		pAIPlayerWaitAction_->ChoiceUpdate();
		pAIPlayerAttackSequence_->SetMyNodeState(NODESTATE::READY);
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
