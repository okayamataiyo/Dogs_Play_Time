#pragma once
//インクルード
#include "StateBase.h"
#include <memory>
#include <string>
#include <map>

class GameObject;

//状態遷移を管理するクラス
class StateManager
{
private:
	std::map < std::string, std::unique_ptr<StateBase>> statesMap_;	//状態を名前で持つマップ（状態の持ち主）
	StateBase* currentState_;	//今の状態（statesMap_の中のものを指しているだけ）
	GameObject* pParent_;		//このStateManagerを使っているオブジェクト（借りているだけ）

public:
	StateManager(GameObject* _pParent);
	~StateManager();

	void Update();
	void ChangeState(const std::string& _stateName);

	//新しい状態を追加する（所有はStateManagerに移る）
	//同じ名前の状態がすでにある場合は、渡された状態は破棄して、すでにある状態を返す
	//戻値：その名前で登録されている状態（StateManagerが消えるまで有効。借りているだけ）
	StateBase* AddState(const std::string& _stateName, std::unique_ptr<StateBase> _state);
	GameObject* GetGameObject() { return pParent_; }
};

