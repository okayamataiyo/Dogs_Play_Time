#pragma once
//インクルード
#include <vector>

class GameObject;

enum class NODESTATE
{
	READY = 0,
	RUNNING,
	SUCCESS,
	FAILURE,
};

class Node
{
public:
	Node(Node* _pParentNode, GameObject* _pGameObject) {};
	NODESTATE GetMyNodeState() { return nodeData_.myNodeState_; }
	void SetMyNodeState(NODESTATE _nodeState) { nodeData_.myNodeState_ = _nodeState; }
	virtual void ChoiceUpdate() = 0;
protected:
	virtual void ReadyUpdate() = 0;
	virtual void RunningUpdate() = 0;
	virtual void SuccessUpdate() = 0;
	virtual void FailureUpdate() = 0;
	
	


	struct NodeData
	{
		NODESTATE myNodeState_;
		NODESTATE parentNodeState_;
		Node* pParentNode_;
		GameObject* pGameObject_;

		NodeData()
			:myNodeState_{NODESTATE::READY},parentNodeState_{NODESTATE::READY}
			,pParentNode_{nullptr},pGameObject_{nullptr}
		{

		}
	}nodeData_;
};