#pragma once
//インクルード
#include "Node.h"

class GameObject;
class AIPlayerAttackDecorator;
class AIPlayerAttackSeeAction;
class AIPlayer;

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
	enum class NODECHILDREN
	{
		ATTACKSEEACTION = 0,
		ATTACKDECORATOR,
	}nodeChildren_;
	AIPlayer* pAIPlayer_;
	AIPlayerAttackDecorator* pAIPlayerAttackDecorator_;
	AIPlayerAttackSeeAction* pAIPlayerAttackSeeAction_;
};

