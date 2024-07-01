#pragma once
//インクルード
#include "BehaviourTreeBase.h"
#include <string>
#include <map>


class GameObject;

class BehaviourTreeManager
{
private:

	std::map < std::string, BehaviourTreeBase*> BehaviourTreesMap_;
	BehaviourTreeBase* currentBefaviourTree_;
	GameObject* pParent_;

public:
	BehaviourTreeManager(GameObject* _pParent);
	~BehaviourTreeManager();

	void Update();
	void ChangeState(const std::string& _behaviourTreeName);
	BehaviourTreeBase* AddState(const std::string& _behaviourTreeName, BehaviourTreeBase* _behaviourTree);
	GameObject* GetGameObject() { return pParent_; }
};