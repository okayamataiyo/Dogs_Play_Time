#pragma once
//インクルード
#include <vector>

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
	Node(Node* _pParentNode);

protected:
	virtual void Update() = 0;
	NODESTATE GetMyNodeState() { return nodeData_.myNodeState_; }

	struct NodeData
	{
		NODESTATE myNodeState_;
		NODESTATE parentNodeState_;
		Node* pParentNode_;
		NodeData()
			:myNodeState_{NODESTATE::READY},parentNodeState_{NODESTATE::READY},pParentNode_{nullptr}
		{

		}
	}nodeData_;
};