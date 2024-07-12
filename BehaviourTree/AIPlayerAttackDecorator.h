#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayerAttackAction;
class AIPlayer;

class AIPlayerAttackDecorator : public Node
{
public:
	AIPlayerAttackDecorator(Node* _pParentNode, GameObject* _pGameObject);
	~AIPlayerAttackDecorator();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:
	AIPlayerAttackAction* pAIPlayerAttackAction_;
	AIPlayer* pAIPlayer_;
};

