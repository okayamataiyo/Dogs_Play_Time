#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"

class  PlayerBase : public GameObject
{
protected:

	enum class PLAYERSTATE
	{
		WAIT = 0,
		WALK,
		RUN,
		JUMP,
		STUN,
	};

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
	float controllerMoveSpeed_;
	float mouseMoveSpeed_;
	bool isDash_;				//�_�b�V�����Ă��邩�ǂ���
	float isFling_;				//�n�ʂ��痣��Ă��邩
	float deadZone_;			//�R���g���[���[�̃f�b�g�]�[��
	float pi_;					//�~����
	float halfPi_;				//�~�����̔���
	float dashSpeed_;
	float walkSpeed_;
	//�������ς��Ɋւ��郁���o�ϐ�
	XMVECTOR vecMove_;
	XMVECTOR vecLength_;
	XMVECTOR vecDot_;
	XMVECTOR vecCross_;
	float length_;
	float dot_;
	float angle_;
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
	bool isStun_;		//�X�^�����Ă��邩�ǂ���
	bool isKnockBack_;	//�m�b�N�o�b�N���Ă��邩�ǂ���
	//�������Ɋւ��郁���o�ϐ�

	//���ǔ���Ɋւ��郁���o�ϐ�
	float distMax_;					//�Փ˓_�܂ł̍ő勗��
	float inTheWall_;				//�ǂɖ��܂��Ă��邩
	float outerWallPosFront_;		//�O�̊O�ǂ̈ʒu
	float outerWallPosBack_;		//���̊O�ǂ̈ʒu
	float outerWallPosLeft_;		//���̊O�ǂ̈ʒu
	float outerWallPosRight_;		//�E�̊O�ǂ̈ʒu
	float rayFloorDistUp_;			//��̂��蔲�����ƃv���C���[�̍�����
	float rayFloorDistDown_;		//���̂��蔲�����ƃv���C���[�̍�����
	float rayStageBlockDistDown_;
	float rayStageDistDown_;		//�n�ʂƃv���C���[�̍�����
	float rayStageDistFront_;		//�O�ǂƃv���C���[�̍�����
	float rayStageDistBack_;		//��ǂƃv���C���[�̍�����
	float rayStageDistLeft_;		//���ǂƃv���C���[�̍�����
	float rayStageDistRight_;		//�E�ǂƃv���C���[�̍�����
protected:
	PlayerBase(GameObject* _parent, std::string _name)
		:GameObject(_parent, _name) {}
	//������
	void Initialize()   override {};

	//�X�V
	void Update()       override {};

	//�`��
	void Draw()         override {};

	//�J��
	void Release()      override {};

	virtual void UpdateReady() = 0;

	virtual void UpdatePlay() = 0;

	virtual void UpdateGameOver() = 0;

	virtual void PlayerFall() = 0;

	virtual void PlayerMove() = 0;

	virtual void PlayerJump() = 0;

	virtual void PlayerJumpPower() = 0;

	virtual void PlayerDive() = 0;

	virtual void PlayerDivePower() = 0;

	virtual void PlayerKnockback() = 0;

	virtual void PlayerRayCast() = 0;

	virtual void Stun(int _timeLimit = 60) = 0;

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual PLAYERSTATE GetPlayerState() = 0;

	virtual int GetPadID() = 0;

	virtual bool IsMoving() = 0;

	virtual int GetModelHandle() = 0;
};

