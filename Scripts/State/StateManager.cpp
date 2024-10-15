#include "StateManager.h"

//�R���X�g���N�^
StateManager::StateManager(GameObject* _pParent)
	:pParent_(_pParent), currentState_(nullptr)
{

}

//�f�X�g���N�^
StateManager::~StateManager()
{
	//�v���O�����I�����A��Ԃ��N���[���A�b�v����
	for (const auto& statePair : statesMap_)
	{
		statePair.second->ExitState();
		delete statePair.second;
	}
}


void StateManager::Update()
{
	if (currentState_ != nullptr)
	{
		currentState_->UpdateState();
	}
}

//��Ԃ̕ύX
void StateManager::ChangeState(const std::string& _stateName)
{
	auto it = statesMap_.find(_stateName);

	if (it != statesMap_.end())
	{
		if (currentState_ != nullptr)
		{
			currentState_->ExitState();
		}

		currentState_ = it->second;
		currentState_->EnterState();
	}
}

//�V������Ԃ�ǉ�
StateBase* StateManager::AddState(const std::string& _stateName, StateBase* _state)
{
	auto it = statesMap_.find(_stateName);

	if (it != statesMap_.end())
	{
		//���ɓ������O�̏�Ԃ����݂���ꍇ�͂��̃|�C���^��Ԃ�
		return it->second;
	}

	//���݂��Ȃ��ꍇ�͐V������Ԃ�o�^
	statesMap_[_stateName] = _state;
	return _state;
}