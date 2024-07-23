//インクルード
#include "AIPlayerWaitSelector.h"
#include "AIPlayerWaitAction.h"
#include "AIPlayerAttackSequence.h"
#include "../Engine/Global.h"

using enum Node::NODESTATE;
using enum WAITSELECTORPRIORITY;

AIPlayerWaitSelector::AIPlayerWaitSelector(Node* _pParentNode,GameObject* _pGameObject)
	:Node(_pParentNode,_pGameObject)
	,pAIPlayerWaitAction_{nullptr},pAIPlayerAttackSequence_{nullptr}
{
	nodeData_.myNodeState_ = READY;
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

void AIPlayerWaitSelector::ReadyUpdate()
{
}

void AIPlayerWaitSelector::RunningUpdate()
{
	nodes_[FIRST] = pAIPlayerAttackSequence_;
	nodes_[SECONDS] = pAIPlayerWaitAction_;

	auto& node = nodes_[priority_];
	const int nextNode = 1;

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
		priority_ = (WAITSELECTORPRIORITY)(((int)priority_ + nextNode) % (int)MAX);
	}
	if (node->GetMyNodeState() == FAILURE)
	{
		node->SetMyNodeState(READY);
		nodeData_.myNodeState_ = FAILURE;
	}
}

void AIPlayerWaitSelector::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerWaitSelector::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}
