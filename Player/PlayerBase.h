#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"

namespace
{
	enum Player {
		collectPlayer = 0,
		attackPlayer,
		playerNum
	};
}

class  PlayerBase : public GameObject
{
protected:
	enum class GAMESTATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	};
	//��UI�Ɋւ��郁���o�ϐ�
	int drawScoreTextX_;
	int drawScoreTextY_;
	int drawScoreNumberX_;
	int drawScoreNumberY_;
	//���Q�[�����o�Ɋւ��郁���o�ϐ�
	int FPS_;
	int timeCounter_;
	int timeLimit_;
	float fallLimit_;
	int score_;			//���_
	int scoreAmount_;	//���_�̗�
	int scoreMax_;		//�ő哾�_��
	int padID_;
	//���T�E���h�Ɋւ��郁���o�ϐ�
	float soundVolume_;
	float soundVolumeHalf_;
	//���v���C���[�ړ��Ɋւ��郁���o�ϐ�
	XMVECTOR CamPositionVec_;
	XMFLOAT3 positionPrev_;		//1�t���[���O�̈ʒu���W
	XMFLOAT3 controllerMoveSpeed_;
	bool isRun_;				//�_�b�V�����Ă��邩�ǂ���
	bool isMove_;
	float isFling_;				//�n�ʂ��痣��Ă��邩
	//�������ς��Ɋւ��郁���o�ϐ�
	XMVECTOR vecMove_;
	XMVECTOR vecCross_;
	XMVECTOR vecDirection_;
	float angle_;
	XMFLOAT3 floDir_;
	float floCameraLen_;
	float floKnockbackLenRecedes_;
	//���W�����v�Ɋւ��郁���o�ϐ�
	float gravity_;				//�d��
	float playerInitPosY_;
	float positionY_;			//�v���C���[��Y���W�ɑ������l
	float positionTempY_;		//y���W��Prev�ɕۑ�����
	float positionPrevY_;		//1���I��y���W��ۑ����Ă���
	float jumpPower_;			//�W�����v�̎��̗�
	bool  isJump_;				//�W�����v���Ă邩���Ă��Ȃ���
	//����т��Ɋւ��郁���o�ϐ�
	float divePower_;			//��т��̎��̗�
	float diveSpeed_;
	bool isDive_;
	bool isDived_;
	int diveTime_;
	int diveDuration_;	//��т����p�����鎞��
	int diveTimeWait_;	//��т����I���܂ł̎���
	//�����蔲�����Ɋւ��郁���o�ϐ�
	bool isOnFloor_;		//���蔲�����Ƀ��C���΂��Ă��邩�ǂ���
	//���ؔ��Ɋւ��郁���o�ϐ�
	const int woodBoxDeath_ = 1;	//�ؔ��������Ȃ������ɖؔ��J�E���g�����炷�ϐ�
	std::string woodBoxNumber_;
	float dotProduct_;
	float angleDegrees_;
	float angleDegreesMax_;	//�W�����v�ɂȂ邩�ǂɂȂ邩�̋��ڂ̕ϐ�
	//���X�^���Ɋւ��郁���o�ϐ�
	int stunTimeCounter_;	//�X�^�����Ă���ǂ̂��炢�o������
	int stunLimit_;		//�X�^�����Ă��鎞��
	int hitStopTime_;		//�q�b�g�X�g�b�v�̎���
	int getUpTime_;		//�X�^�����Ă���N���オ�鎞��
	float knockbackSpeed_;
	XMVECTOR vecKnockbackDirection_;
	bool isStun_;		//�X�^�����Ă��邩�ǂ���
	bool isKnockBack_;	//�m�b�N�o�b�N���Ă��邩�ǂ���
	//�������Ɋւ��郁���o�ϐ�

	//���ǔ���Ɋւ��郁���o�ϐ�
	float distMax_;					//�Փ˓_�܂ł̍ő勗��
	float inTheWall_;				//�ǂɖ��܂��Ă��邩
	float rayFloorDistDown_;
	float rayStageDistDown_;
	//���A�j���[�V�����Ɋւ��郁���o�ϐ�
	int startFrame_;
	int endFrame_;
	float animSpeed_;
protected:
	PlayerBase(GameObject* _parent, std::string _name);
	//������
	void Initialize()   override;

	//�X�V
	void Update()       override;

	//�`��
	void Draw()         override;

	//�J��
	void Release()      override;

	virtual void UpdateReady() = 0;

	virtual void UpdatePlay() = 0;

	virtual void UpdateGameOver() = 0;

	virtual void PlayerWaitStateFunc();
	virtual void PlayerWalkStateFunc();
	virtual void PlayerRunStateFunc();
	virtual void PlayerJumpStateFunc();
	virtual void PlayerStunStateFunc();

	virtual void PlayerCamera();

	virtual void PlayerFall() = 0;

	virtual void PlayerMove() = 0;

	virtual void PlayerJump() = 0;

	virtual void PlayerJumpPower() = 0;

	virtual void PlayerDive() = 0;

	virtual void PlayerDivePower() = 0;

	virtual void PlayerKnockback() = 0;

	virtual void PlayerRayCast() = 0;

	virtual void PlayerRevival();

	virtual void PlayerStun(int _timeLimit = 60) = 0;

	virtual void IsMove();

	virtual void IsJump();

	virtual void IsRun();

	virtual void IsStun();

	virtual void IsDive();

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) = 0;

	virtual bool GetIsMove() = 0;

	virtual bool GetIsJump() = 0;

	virtual bool GetIsRun() = 0;

	virtual bool GetIsStun() = 0;

	virtual bool GetIsDive() = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual int GetPadID() = 0;

	virtual int GetModelHandle() = 0;
};

