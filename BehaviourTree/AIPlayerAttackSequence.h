#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayerWaitAction;
class AIPlayerAttackDecorator;

class AIPlayerAttackSequence : public Node
{
public:
	AIPlayerAttackSequence(Node* _pParentNode, GameObject* _pGameObject);
	~AIPlayerAttackSequence();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:

	AIPlayerWaitAction* pAIPlayerWaitAction_;
	AIPlayerAttackDecorator* pAIPlayerAttackDecorator_;
};

