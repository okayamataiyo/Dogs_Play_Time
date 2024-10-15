#pragma once
class StateManager;

//��ԃx�[�X�N���X
class StateBase
{
protected:
	StateManager* pStateManager_;
public:
	StateBase(StateManager* _manager)
		:pStateManager_(_manager)
	{

	}

	//�f�X�g���N�^
	virtual ~StateBase() {};
	//��Ԃɓ��鎞�̏�����
	virtual void EnterState() = 0;
	//��Ԃ̍X�V
	virtual void UpdateState() = 0;
	//��Ԃ���o�鎞�̌㏈��
	virtual void ExitState() = 0;
};