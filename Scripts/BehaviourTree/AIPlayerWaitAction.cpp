#include "AIPlayerWaitAction.h"

using enum NODESTATE;

AIPlayerWaitAction::AIPlayerWaitAction(Node* _pParentNode,GameObject* _pGameObject)
	:Node(_pParentNode,_pGameObject)
{
	nodeData_.myNodeState_ = READY;
	nodeData_.pParentNode_ = _pParentNode;
	nodeData_.pGameObject_ = _pGameObject;
}

void AIPlayerWaitAction::ChoiceUpdate()
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

void AIPlayerWaitAction::ReadyUpdate()
{
}

void AIPlayerWaitAction::RunningUpdate()
{
	nodeData_.myNodeState_ = SUCCESS;
}

void AIPlayerWaitAction::SuccessUpdate()
{
	nodeData_.myNodeState_ = READY;
}

void AIPlayerWaitAction::FailureUpdate()
{
	nodeData_.myNodeState_ = READY;
}