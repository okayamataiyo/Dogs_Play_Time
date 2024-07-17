#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayerAttackDiveAction;
class AIPlayer;

class AIPlayerAttackDiveDecorator : public Node
{
public:
	AIPlayerAttackDiveDecorator(Node* _pParentNode, GameObject* _pGameObject);
	~AIPlayerAttackDiveDecorator();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:
	bool isDive_;
	AIPlayerAttackDiveAction* pAIPlayerAttackDiveAction_;
	AIPlayer* pAIPlayer_;
};

