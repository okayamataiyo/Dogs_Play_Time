#include "Node.h"

Node::Node(Node* _node, std::vector<Node*> _nodeChildren, Work* _work)
	:myNode_(_node),nodeChildren_(_nodeChildren),myWork_(_work)
{
}

NodeState Node::Run()
{
	auto result = myWork_->operator()();
	switch (result)
	{
	case NodeState::READY:
		break;
	case NodeState::RUNNING:
		break;
	case NodeState::SUCCESS:
		break;
	case NodeState::FAILURE:
		break;
	}
}
