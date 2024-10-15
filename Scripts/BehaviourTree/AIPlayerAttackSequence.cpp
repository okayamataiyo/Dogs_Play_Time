//インクルード
#include "AIPlayerAttackSequence.h"
#include "AIPlayerAttackDecorator.h"
#include "AIPlayerAttackSeeAction.h"
#include "AIPlayerAttackDiveDecorator.h"
#include "../Player/AIPlayer.h"
#include "../../Engine/Global.h"

using enum Node::NODESTATE;
using enum ATTACKSEQUENCEPRIORITY;

AIPlayerAttackSequence::AIPlayerAttackSequence(Node* _pParentNode, GameObject* _pGameObject)
	:Node(_pParentNode, _pGameObject)
	,priority_{FIRST}
	,pAIPlayerAttackDecorator_{ nullptr },pAIPlayerAttackSeeAction_{nullptr},pAIPlayerAttackDiveDecorator_{nullptr}
{
	nodeData_.myNodeState_ = READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
	pAIPlayerAttackDecorator_ = new AIPlayerAttackDecorator(this, nodeData_.pGameObject_);
	pAIPlayerAttackSeeAction_ = new AIPlayerAttackSeeAction(this, nodeData_.pGameObject_);
	pAIPlayerAttackDiveDecorator_ = new AIPlayerAttackDiveDecorator(this, nodeData_.pGameObject_);
	nodes_[FIRST] = pAIPlayerAttackSeeAction_;
	nodes_[SECONDS] = pAIPlayerAttackDecorator_;
	nodes_[THIRD] = pAIPlayerAttackDiveDecorator_;
}

AIPlayerAttackSequence::~AIPlayerAttackSequence()
{
	SAFE_DELETE(pAIPlayerAttackDecorator_);
	SAFE_DELETE(pAIPlayerAttackSeeAction_);
	SAFE_DELETE(pAIPlayerAttackDiveDecorator_);
}

void AIPlayerAttackSequence::ChoiceUpdate()
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

void AIPlayerAttackSequence::ReadyUpdate()
{
}

void AIPlayerAttackSequence::RunningUpdate()
{
	auto& node = nodes_[priority_];
	if (node->GetMyNodeState() == READY)
	{
		node->SetMyNodeState(RUNNING);
	}
	if (node->GetMyNodeState() == RUNNING)
	{
		node->ChoiceUpdate();
	}
	if (node->GetMyNodeState() == SUCCESS)
	{
		node->SetMyNodeState(READY);
		nodeData_.myNodeState_ = SUCCESS;
		priority_ = (ATTACKSEQUENCEPRIORITY)(((int)priority_ + 1) % (int)MAX);
	}
	if (node->GetMyNodeState() == FAILURE)
	{
		node->SetMyNodeState(READY);
		nodeData_.myNodeState_ = FAILURE;
	}
}

void AIPlayerAttackSequence::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerAttackSequence::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}