#pragma once
//インクルード
#include "Node.h"

class GameObject;

class AIPlayerWaitAction : public Node
{
public:
	AIPlayerWaitAction(Node* _pParentNode,GameObject* _pGameObject);
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;
private:
};

