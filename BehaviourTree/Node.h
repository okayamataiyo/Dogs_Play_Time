#pragma once
//インクルード
#include "Work.h"
//親子関係と働きを持ったNode

class Node
{
public:
	Node(Node* _node,std::vector<Node*> _nodeChildren, Work* _work);
	NodeState Run();

private:
	Node* myNode_;
	std::vector<Node*> nodeChildren_;
	Work* myWork_;
};