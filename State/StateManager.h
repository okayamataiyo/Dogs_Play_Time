#pragma once
//インクルード
#include "StateBase.h"
#include <string>
#include <map>

class GameObject;

//状態遷移を管理するクラス
class StateManager
{
private:
	std::map < std::string, StateBase*> statesMap_;	//状態を名前で管理するマップ
	StateBase* currentState_;
	GameObject* pParent_;

public:
	StateManager(GameObject* _pParent);
	~StateManager();

	void Update();
	void ChangeState(const std::string& _stateName);
	StateBase* AddState(const std::string& _stateName, StateBase* _state);
	GameObject* GetGameObject() { return pParent_; }
};

