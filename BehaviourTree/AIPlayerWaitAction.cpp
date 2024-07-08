#include "AIPlayerWaitAction.h"

AIPlayerWaitAction::AIPlayerWaitAction(Node* _pParentNode,GameObject* _pGameObject)
	:Node(_pParentNode,_pGameObject)
{
	nodeData_.myNodeState_ = NODESTATE::READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
}

void AIPlayerWaitAction::ChoiceUpdate()
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

void AIPlayerWaitAction::ReadyUpdate()
{
}

void AIPlayerWaitAction::RunningUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::SUCCESS;
}

void AIPlayerWaitAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}

void AIPlayerWaitAction::FailureUpdate()
{
	nodeData_.myNodeState_ = NODESTATE::READY;
}