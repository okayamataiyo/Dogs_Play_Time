#pragma once
//�C���N���[�h
#include "Work.h"
//�e�q�֌W�Ɠ�����������Node

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