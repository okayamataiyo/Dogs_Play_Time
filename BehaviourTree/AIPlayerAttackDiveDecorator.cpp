//�C���N���[�h
#include <random>
#include "AIPlayerAttackDiveDecorator.h"
#include "AIPlayerAttackDiveAction.h"
#include "../Player/AIPlayer.h"
#include "../Engine/Global.h"

using enum NODESTATE;

AIPlayerAttackDiveDecorator::AIPlayerAttackDiveDecorator(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	,isDive_{false}
	, pAIPlayerAttackDiveAction_(nullptr), pAIPlayer_((AIPlayer*)_pGameObject)
{
	nodeData_.myNodeState_ = READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerAttackDiveAction_ = new AIPlayerAttackDiveAction(this, _pGameObject);
}

AIPlayerAttackDiveDecorator::~AIPlayerAttackDiveDecorator()
{
	SAFE_DELETE(pAIPlayerAttackDiveAction_);
}

void AIPlayerAttackDiveDecorator::ChoiceUpdate()
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

void AIPlayerAttackDiveDecorator::ReadyUpdate()
{

}

void AIPlayerAttackDiveDecorator::RunningUpdate()
{
	//����������̐ݒ�
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	//1����2�܂ł̃����_���Ȓl�̍쐬
	isDive_ = dis(gen);
	if (isDive_ == true)
	{
		pAIPlayerAttackDiveAction_->SetMyNodeState(RUNNING);
		nodeData_.myNodeState_ = pAIPlayerAttackDiveAction_->GetMyNodeState();
	}
	else
	{
		nodeData_.myNodeState_ = FAILURE;
	}
	if (pAIPlayerAttackDiveAction_->GetMyNodeState() == RUNNING)
	{
		pAIPlayerAttackDiveAction_->ChoiceUpdate();
	}
	if (pAIPlayerAttackDiveAction_->GetMyNodeState() == SUCCESS)
	{
		pAIPlayerAttackDiveAction_->SetMyNodeState(READY);
		nodeData_.myNodeState_ = SUCCESS;
	}
	if (pAIPlayerAttackDiveAction_->GetMyNodeState() == FAILURE)
	{
		pAIPlayerAttackDiveAction_->SetMyNodeState(READY);
		nodeData_.myNodeState_ = FAILURE;
	}
}

void AIPlayerAttackDiveDecorator::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerAttackDiveDecorator::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}