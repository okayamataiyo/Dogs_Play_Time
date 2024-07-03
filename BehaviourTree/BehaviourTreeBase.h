#pragma once
class BehaviourTreeManager;

namespace
{
	enum class TREE
	{
		READY = 0,
		SUCCESS,
		FAILURE,
		RUNNING,
	};
}

//AI�̃A���S���Y����1��
class BehaviourTreeBase
{
protected:
	BehaviourTreeManager* pBehaviourTreeManager_;
public:
	BehaviourTreeBase(BehaviourTreeManager* _manager)
		:pBehaviourTreeManager_(_manager)
	{

	}

	//�f�X�g���N�^
	virtual ~BehaviourTreeBase() {};
	//��Ԃɓ��鎞�̏�����
	virtual void EnterState() = 0;
	//��Ԃ̍X�V
	virtual void UpdateState() = 0;
	//��Ԃ���o�鎞�̌㏈��
	virtual void ExitState() = 0;
	virtual TREE Action() = 0;
	virtual TREE Selector() = 0;
	virtual TREE Sequence() = 0;
	virtual TREE Decorator() = 0;

};