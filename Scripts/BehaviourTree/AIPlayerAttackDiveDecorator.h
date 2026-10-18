#pragma once
//インクルード
#include "Node.h"
#include <memory>

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
	std::unique_ptr<AIPlayerAttackDiveAction> pAIPlayerAttackDiveAction_;	//子のノード（このノードが持つ）
	AIPlayer* pAIPlayer_;
};

