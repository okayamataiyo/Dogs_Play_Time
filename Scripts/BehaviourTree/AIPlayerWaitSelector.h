#pragma once
//インクルード
#include "Node.h"
#include <memory>
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
	std::unique_ptr<AIPlayerWaitAction> pAIPlayerWaitAction_;	//子のノード（このノードが持つ）
	std::unique_ptr<AIPlayerAttackSequence> pAIPlayerAttackSequence_;	//子のノード（このノードが持つ）
	WAITSELECTORPRIORITY priority_;
	std::map<WAITSELECTORPRIORITY, Node*> nodes_;
};

