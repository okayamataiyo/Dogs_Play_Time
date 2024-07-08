#pragma once
//インクルード
#include "../BehaviourTree/Node.h"

class AIPlayerWaitSelector : public Node
{
public:
	AIPlayerWaitSelector(Node* _pParentNode);

private:
	void Update() override;
};

