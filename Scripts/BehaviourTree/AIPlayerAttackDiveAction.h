#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayer;

class AIPlayerAttackDiveAction : public Node
{
public:
	AIPlayerAttackDiveAction(Node* _pParentNode, GameObject* _pGameObject);
	~AIPlayerAttackDiveAction();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:
	AIPlayer* pAIPlayer_;
};

