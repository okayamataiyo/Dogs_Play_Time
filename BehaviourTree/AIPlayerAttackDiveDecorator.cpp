//�C���N���[�h
#include <random>
#include "AIPlayerAttackDiveDecorator.h"
#include "AIPlayerAttackDiveAction.h"
#include "../Player/AIPlayer.h"
#include "../Engine/Global.h"

using enum Node::NODESTATE;

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
	isDive_ = (bool)dis(gen);

	auto& node = pAIPlayerAttackDiveAction_;
	if (isDive_)
	{
		node->SetMyNodeState(RUNNING);
		nodeData_.myNodeState_ = node->GetMyNodeState();
	}
	else
	{
		nodeData_.myNodeState_ = FAILURE;
	}
	if (node->GetMyNodeState() == RUNNING)
	{
		node->ChoiceUpdate();
	}
	if (node->GetMyNodeState() == SUCCESS)
	{
		node->SetMyNodeState(READY);
		nodeData_.myNodeState_ = SUCCESS;
	}
	if (node->GetMyNodeState() == FAILURE)
	{
		node->SetMyNodeState(READY);
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