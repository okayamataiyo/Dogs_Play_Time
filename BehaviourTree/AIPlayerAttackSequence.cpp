//インクルード
#include "AIPlayerAttackSequence.h"
#include "AIPlayerWaitAction.h"
#include "AIPlayerAttackDecorator.h"
#include "../Engine/Global.h"

AIPlayerAttackSequence::AIPlayerAttackSequence(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	, pAIPlayerWaitAction_{ nullptr }, pAIPlayerAttackDecorator_{ nullptr }
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerWaitAction_ = new AIPlayerWaitAction(this, nodeData_.pGameObject_);
	pAIPlayerAttackDecorator_ = new AIPlayerAttackDecorator(this, nodeData_.pGameObject_);
}

AIPlayerAttackSequence::~AIPlayerAttackSequence()
{
	SAFE_DELETE(pAIPlayerWaitAction_);
	SAFE_DELETE(pAIPlayerAttackDecorator_);
}

void AIPlayerAttackSequence::ChoiceUpdate()
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

void AIPlayerAttackSequence::ReadyUpdate()
{
}

void AIPlayerAttackSequence::RunningUpdate()
{
	if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::READY)
	{
		pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::RUNNING);
		pAIPlayerAttackDecorator_->ChoiceUpdate();
	}
	if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::RUNNING)
	{
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

void AIPlayerAttackSequence::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerAttackSequence::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}
