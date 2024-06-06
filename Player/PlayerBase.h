#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"

namespace
{
	enum class PLAYERSTATE {
		COLLECTPLAYER = 0,
		ATTACKPLAYER,
		PLAYERNUM
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
	//���Q�[�����o�Ɋւ��郁���o�ϐ�
	struct GameData
	{
		int FPS_;
		int timeCounter_;
		int timeLimit_;
		float fallLimit_;
		int score_;			//���_
		int scoreAmount_;	//���_�̗�
		int scoreMax_;		//�ő哾�_��
		int scoreTimeCounter_;	//���_�����Z���鎞��
		int scoreTimeCounterWait_;	//���_�����Z����ҋ@����
		int padID_;
		int walkOrFight_;
		GameData()
			:FPS_{ 600 }, timeCounter_{ 0 }, timeLimit_{ 60 }, fallLimit_{ 100.0f }, score_{ 0 }, scoreAmount_{ 10 }, scoreMax_{ 150 },scoreTimeCounter_{0},scoreTimeCounterWait_{30}, padID_{0}, walkOrFight_{false}
		{

		}
	}gameData_;
	//���T�E���h�Ɋւ��郁���o�ϐ�
	struct SoundData
	{
		float soundVolume_;
		float soundVolumeHalf_;
		SoundData()
			:soundVolume_{ 0.5f }, soundVolumeHalf_{ soundVolume_ / 2 }
		{

		}
	}soundData_;
	//���v���C���[�ړ��Ɋւ��郁���o�ϐ�
	struct MoveData
	{
		XMVECTOR CamPositionVec_;
		XMFLOAT3 positionPrev_;		//1�t���[���O�̈ʒu���W
		XMFLOAT3 padMoveSpeed_;
		bool isRun_;				//�_�b�V�����Ă��邩�ǂ���
		bool isMove_;
		float isFling_;				//�n�ʂ��痣��Ă��邩
		MoveData()
			:CamPositionVec_{}, positionPrev_{ 0.0f,0.0f,0.0f }, padMoveSpeed_{ 0.3f,0.0f,0.3f }, isRun_{ false }, isMove_{ false }, isFling_{ 1.0f }
		{

		}
	}moveData_;
	//�������ς��Ɋւ��郁���o�ϐ�
	struct DirData
	{
		XMVECTOR vecMove_;
		XMVECTOR vecCross_;
		XMVECTOR vecDirection_;
		float angle_;
		XMFLOAT3 vecCam_;
		DirData()
			:vecMove_{}, vecCross_{}, vecDirection_{}, angle_{ 0.0f }, vecCam_{ 0.0f,5.0f,-10.0f }
		{

		}
	}dirData_;
	//���W�����v�Ɋւ��郁���o�ϐ�
	struct JumpData
	{
		float gravity_;				//�d��
		float playerInitPosY_;
		float positionY_;			//�v���C���[��Y���W�ɑ������l
		float positionTempY_;		//y���W��Prev�ɕۑ�����
		float positionPrevY_;		//1���I��y���W��ۑ����Ă���
		float jumpPower_;			//�W�����v�̎��̗�
		bool  isJump_;				//�W�����v���Ă邩���Ă��Ȃ���
		JumpData()
			:gravity_{ 0.007f }, playerInitPosY_{0.6f}, positionY_{}, positionTempY_{ 0.0f }, positionPrevY_{ 0.0f }, jumpPower_{ 0.3f }, isJump_{ false }
		{

		}
	}jumpData_;
	//����т��Ɋւ��郁���o�ϐ�
	struct DiveData
	{
		float divePower_;			//��т��̎��̗�
		float diveSpeed_;
		bool isDive_;
		bool isDived_;
		int diveTime_;
		int diveDuration_;	//��т����p�����鎞��
		int diveTimeWait_;	//��т����I���܂ł̎���
		DiveData()
			:divePower_{ 0.1f }, diveSpeed_{ 0.6f }, isDive_{ false }, isDived_{ false }, diveTime_{ 0 }, diveDuration_{ 1 }, diveTimeWait_{ 30 }
		{

		}
	}diveData_;
	//�����蔲�����Ɋւ��郁���o�ϐ�
	struct FloorData
	{
		bool isOnFloor_;		//���蔲�����Ƀ��C���΂��Ă��邩�ǂ���
		FloorData()
			:isOnFloor_{ false }
		{

		}
	}floorData_;
	//���ؔ��Ɋւ��郁���o�ϐ�
	struct WoodBoxData
	{
		const int woodBoxDeath_ = 1;	//�ؔ��������Ȃ������ɖؔ��J�E���g�����炷�ϐ�
		std::string woodBoxNumber_;
		float dotProduct_;
		float angleDegrees_;
		float angleDegreesMax_;	//�W�����v�ɂȂ邩�ǂɂȂ邩�̋��ڂ̕ϐ�
		WoodBoxData()
			:woodBoxNumber_{ "" }, dotProduct_{ 0.0f }, angleDegrees_{ 0.0f }, angleDegreesMax_{ 80.0f }
		{

		}
	}woodBoxData_;
	//���X�^���Ɋւ��郁���o�ϐ�
	struct StunData
	{
		int stunTimeCounter_;	//�X�^�����Ă���ǂ̂��炢�o������
		int stunLimit_;		//�X�^�����Ă��鎞��
		int hitStopTime_;		//�q�b�g�X�g�b�v�̎���
		int getUpTime_;		//�X�^�����Ă���N���オ�鎞��
		float knockbackSpeed_;
		XMVECTOR vecKnockbackDirection_;
		bool isStun_;		//�X�^�����Ă��邩�ǂ���
		bool isKnockBack_;	//�m�b�N�o�b�N���Ă��邩�ǂ���
		StunData()
			:stunTimeCounter_{ 0 }, stunLimit_{ 0 }, hitStopTime_{ 10 }, getUpTime_{ 30 }, knockbackSpeed_{ 0.3f }, vecKnockbackDirection_{}, isStun_{ false }, isKnockBack_{ false }
		{

		}
	}stunData_;
	//�������Ɋւ��郁���o�ϐ�

	//���ǔ���Ɋւ��郁���o�ϐ�
	struct WallData
	{
		float distMax_;					//�Փ˓_�܂ł̍ő勗��
		float inTheWall_;				//�ǂɖ��܂��Ă��邩
		float rayFloorDistDown_;
		float rayStageDistDown_;
		WallData()
			:distMax_{ 99999.0f }, inTheWall_{ 1.5f }, rayFloorDistDown_{ 0.0f }, rayStageDistDown_{ 0.0f }
		{

		}
	}wallData_;
	//�����Ɋւ��郁���o�ϐ�
	struct BoneData
	{
		int decBoneCount_;
		bool isBoneTatch_;
		int killTime_;
		int killTimeWait_;
		int killTimeMax_;
		BoneData()
			:decBoneCount_{ -1 }, isBoneTatch_{ false }, killTime_{ 99999 }, killTimeWait_{ 30 }, killTimeMax_{ 99999 }
		{

		}
	}boneData_;
	//���A�j���[�V�����Ɋւ��郁���o�ϐ�
	struct AnimData
	{
		int startFrame_;
		int endFrame_;
		float animSpeed_;
		AnimData()
			:startFrame_{ 0 }, endFrame_{ 0 }, animSpeed_{ 0.0f }
		{

		}
	}animData_;
protected:
	PlayerBase(GameObject* _parent, std::string _name);
	//������
	void Initialize()   override;

	//�X�V
	void Update()       override;

	//�`��
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;

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

	virtual void PlayerScore();

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
	
	void ApplyMovement(float moveX, float moveZ);

	virtual void IsMove();

	virtual void IsJump();

	virtual void IsRun();

	virtual void IsStun();

	virtual void IsDive();

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f);

	virtual bool GetIsMove() = 0;

	virtual bool GetIsJump() = 0;

	virtual bool GetIsRun() = 0;

	virtual bool GetIsStun() = 0;

	virtual bool GetIsDive() = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual int GetPadID() = 0;

	virtual int GetModelHandle() = 0;
};

