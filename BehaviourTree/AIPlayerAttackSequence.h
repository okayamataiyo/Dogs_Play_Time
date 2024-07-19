#pragma once
//インクルード
#include "Node.h"
#include <map>

class GameObject;
class AIPlayerAttackDecorator;
class AIPlayerAttackSeeAction;
class AIPlayerAttackDiveDecorator;

enum class ATTACKSEQUENCEPRIORITY
{
	FIRST = 0,
	SECONDS,
	THIRD,
	MAX,
};

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
	AIPlayerAttackDecorator* pAIPlayerAttackDecorator_;
	AIPlayerAttackSeeAction* pAIPlayerAttackSeeAction_;
	AIPlayerAttackDiveDecorator* pAIPlayerAttackDiveDecorator_;
	ATTACKSEQUENCEPRIORITY priority_;
	std::map<ATTACKSEQUENCEPRIORITY, Node*> nodes_;
};