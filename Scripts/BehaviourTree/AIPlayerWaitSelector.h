#pragma once
//インクルード
#include "Node.h"
#include <map>

enum class WAITSELECTORPRIORITY
{
	FIRST = 0,
	SECONDS,
	MAX,
};

class GameObject;
class AIPlayerWaitAction;
class AIPlayerAttackSequence;

class AIPlayerWaitSelector : public Node
{
public:
	AIPlayerWaitSelector(Node* _pParentNode,GameObject* _pGameObject);
	~AIPlayerWaitSelector();
	void ChoiceUpdate() override;
	void ReadyUpdate() override;
	void RunningUpdate() override;
	void SuccessUpdate() override;
	void FailureUpdate() override;

private:
	AIPlayerWaitAction* pAIPlayerWaitAction_;
	AIPlayerAttackSequence* pAIPlayerAttackSequence_;
	WAITSELECTORPRIORITY priority_;
	std::map<WAITSELECTORPRIORITY, Node*> nodes_;
};

