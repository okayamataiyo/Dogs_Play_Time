//インクルード
#include "AIPlayerAttackDecorator.h"
#include "AIPlayerAttackAction.h"
#include "../Player/AIPlayer.h"
#include "../Engine/Global.h"

AIPlayerAttackDecorator::AIPlayerAttackDecorator(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	,pAIPlayerAttackAction_(nullptr),pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = NODESTATE::READY;
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

void AIPlayerAttackDecorator::ReadyUpdate()
{

}

void AIPlayerAttackDecorator::RunningUpdate()
{
	if (pAIPlayer_->GetIsAttack())
	{
		pAIPlayerAttackAction_->SetMyNodeState(NODESTATE::RUNNING);
		pAIPlayerAttackAction_->ChoiceUpdate();
		nodeData_.myNodeState_ = pAIPlayerAttackAction_->GetMyNodeState();
	}
	else
	{
		nodeData_.myNodeState_ = NODESTATE::FAILURE;
	}
	if (pAIPlayerAttackAction_->GetMyNodeState() == NODESTATE::SUCCESS)
	{
		pAIPlayerAttackAction_->SetMyNodeState(NODESTATE::READY);
		nodeData_.myNodeState_ = NODESTATE::SUCCESS;
	}
}

void AIPlayerAttackDecorator::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerAttackDecorator::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}