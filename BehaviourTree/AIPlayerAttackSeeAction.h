#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayer;

class AIPlayerAttackSeeAction : public Node
{
public:
	AIPlayerAttackSeeAction(Node* _pParentNode, GameObject* _pGameObject);
	~AIPlayerAttackSeeAction();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:
	AIPlayer* pAIPlayer_;
};

