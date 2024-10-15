#pragma once
//�C���N���[�h
#include "StateBase.h"
#include <string>
#include <map>

class GameObject;

//��ԑJ�ڂ��Ǘ�����N���X
class StateManager
{
private:
	std::map < std::string, StateBase*> statesMap_;	//��Ԃ𖼑O�ŊǗ�����}�b�v
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

