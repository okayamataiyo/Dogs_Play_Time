#include "BehaviourTreeManager.h"

//�R���X�g���N�^
BehaviourTreeManager::BehaviourTreeManager(GameObject* _pParent)
	:pParent_(_pParent),currentBefaviourTree_(nullptr)
{
}

//�f�X�g���N�^
BehaviourTreeManager::~BehaviourTreeManager()
{
	//�v���O�����I�����A��Ԃ��N���[���A�b�v����
	for (const auto& statePair : BehaviourTreesMap_)
	{
		statePair.second->ExitState();
		delete statePair.second;
	}
}

void BehaviourTreeManager::Update()
{
	if (currentBefaviourTree_ != nullptr)
	{
		currentBefaviourTree_->UpdateState();
	}
}

//��Ԃ̕ύX
void BehaviourTreeManager::ChangeState(const std::string& _behaviourTreeName)
{
	auto it = BehaviourTreesMap_.find(_behaviourTreeName);

	if (it != BehaviourTreesMap_.end())
	{
		if (currentBefaviourTree_ != nullptr)
		{
			currentBefaviourTree_->ExitState();
		}

		currentBefaviourTree_ = it->second;
		currentBefaviourTree_->EnterState();
	}
}

//�V������Ԃ�ǉ�
BehaviourTreeBase* BehaviourTreeManager::AddState(const std::string& _behaviourTreeName, BehaviourTreeBase* _behaviourTree)
{
	auto it = BehaviourTreesMap_.find(_behaviourTreeName);

	if (it != BehaviourTreesMap_.end())
	{
		//���ɓ������O�̏�Ԃ����݂���ꍇ�͂��̃|�C���^��Ԃ�
		return it->second;
	}

	//���݂��Ȃ��ꍇ�͐V������Ԃ�o�^
	BehaviourTreesMap_[_behaviourTreeName] = _behaviourTree;
	return _behaviourTree;
}
