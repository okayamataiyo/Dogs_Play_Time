//インクルード
#include "AIPlayerAttackDecorator.h"
#include "AIPlayerAttackAction.h"
#include "../Player/AIPlayer.h"
#include "../../Engine/Global.h"

using enum Node::NODESTATE;

AIPlayerAttackDecorator::AIPlayerAttackDecorator(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	,pAIPlayerAttackAction_(nullptr),pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerAttackAction_ = new AIPlayerAttackAction(this, _pGameObject);
}

AIPlayerAttackDecorator::~AIPlayerAttackDecorator()
{
	SAFE_DELETE(pAIPlayerAttackAction_);
}

void AIPlayerAttackDecorator::ChoiceUpdate()
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

void AIPlayerAttackDecorator::ReadyUpdate()
{

}

void AIPlayerAttackDecorator::RunningUpdate()
{
	if (pAIPlayer_->GetIsAttack())
	{
		pAIPlayerAttackAction_->SetMyNodeState(RUNNING);
		nodeData_.myNodeState_ = pAIPlayerAttackAction_->GetMyNodeState();
	}
	else
	{
		nodeData_.myNodeState_ = FAILURE;
	}
	if (pAIPlayerAttackAction_->GetMyNodeState() == RUNNING)
	{
		pAIPlayerAttackAction_->ChoiceUpdate();
	}
	if (pAIPlayerAttackAction_->GetMyNodeState() == SUCCESS)
	{
		pAIPlayerAttackAction_->SetMyNodeState(READY);
		nodeData_.myNodeState_ = SUCCESS;
	}
	if (pAIPlayerAttackAction_->GetMyNodeState() == FAILURE)
	{
		pAIPlayerAttackAction_->SetMyNodeState(READY);
		nodeData_.myNodeState_ = FAILURE;
	}
}

void AIPlayerAttackDecorator::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerAttackDecorator::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}