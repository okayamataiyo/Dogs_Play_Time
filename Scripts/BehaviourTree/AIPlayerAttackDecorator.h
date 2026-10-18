#pragma once
//インクルード
#include "Node.h"
#include <memory>

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
	std::unique_ptr<AIPlayerAttackAction> pAIPlayerAttackAction_;	//子のノード（このノードが持つ）
	AIPlayer* pAIPlayer_;
};

