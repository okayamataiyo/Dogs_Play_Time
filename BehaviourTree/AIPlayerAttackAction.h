#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayer;

class AIPlayerAttackAction : public Node
{
public:
	AIPlayerAttackAction(Node* _pParentNode, GameObject* _pGameObject);
	~AIPlayerAttackAction();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:
	AIPlayer* pAIPlayer_;
};

