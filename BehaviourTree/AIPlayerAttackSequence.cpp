//インクルード
#include "AIPlayerAttackSequence.h"
#include "AIPlayerAttackDecorator.h"
#include "AIPlayerAttackSeeAction.h"
#include "../Player/AIPlayer.h"
#include "../Engine/Global.h"

AIPlayerAttackSequence::AIPlayerAttackSequence(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	,nodeChildren_{NODECHILDREN::ATTACKSEEACTION}
	,pAIPlayerAttackDecorator_{ nullptr },pAIPlayerAttackSeeAction_{nullptr}
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerAttackDecorator_ = new AIPlayerAttackDecorator(this, nodeData_.pGameObject_);
	pAIPlayerAttackSeeAction_ = new AIPlayerAttackSeeAction(this, nodeData_.pGameObject_);
}

AIPlayerAttackSequence::~AIPlayerAttackSequence()
{
	SAFE_DELETE(pAIPlayerAttackDecorator_);
	SAFE_DELETE(pAIPlayerAttackSeeAction_);
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
	switch(nodeChildren_)
	{
	case NODECHILDREN::ATTACKSEEACTION:
		if (pAIPlayerAttackSeeAction_->GetMyNodeState() == NODESTATE::READY)
		{
			pAIPlayerAttackSeeAction_->SetMyNodeState(NODESTATE::RUNNING);
		}
		if (pAIPlayerAttackSeeAction_->GetMyNodeState() == NODESTATE::RUNNING)
		{
			pAIPlayerAttackSeeAction_->ChoiceUpdate();
		}
		if (pAIPlayerAttackSeeAction_->GetMyNodeState() == NODESTATE::SUCCESS)
		{
			pAIPlayerAttackSeeAction_->SetMyNodeState(NODESTATE::READY);
			nodeChildren_ = NODECHILDREN::ATTACKDECORATOR;
			nodeData_.myNodeState_ = NODESTATE::SUCCESS;
		}
		if (pAIPlayerAttackSeeAction_->GetMyNodeState() == NODESTATE::FAILURE)
		{
			pAIPlayerAttackSeeAction_->SetMyNodeState(NODESTATE::READY);
			nodeData_.myNodeState_ = NODESTATE::FAILURE;
		}
		break;
	case NODECHILDREN::ATTACKDECORATOR:
		if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::READY)
		{
			pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::RUNNING);
		}
		if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::RUNNING)
		{
			pAIPlayerAttackDecorator_->ChoiceUpdate();
			nodeData_.myNodeState_ = pAIPlayerAttackDecorator_->GetMyNodeState();
		}
		if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::SUCCESS)
		{
			pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::READY);
			nodeChildren_ = NODECHILDREN::ATTACKSEEACTION;
			nodeData_.myNodeState_ = NODESTATE::SUCCESS;
		}
		if (pAIPlayerAttackDecorator_->GetMyNodeState() == NODESTATE::FAILURE)
		{
			pAIPlayerAttackDecorator_->SetMyNodeState(NODESTATE::READY);
			nodeData_.myNodeState_ = NODESTATE::FAILURE;
		}
		break;
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
