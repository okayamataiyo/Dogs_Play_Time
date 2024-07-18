#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;
class CollectPlayer;
class AttackPlayer;
class SphereCollider;
class WoodBox;
class Text;
class Stage;
class Floor;
class SceneManager;
class ItemObjectManager;
class StateManager;
class ImageManager;
class ParticleManager;
class UIManager;
class AIPlayerWaitSelector;

namespace
{
	std::string soundAIPlayerNames[] =
	{
		"Stun",
		"Walk",
		"Jump",
		"Run",
	};

	std::string aIPlayerName = "AIPlayer";
}

/// <summary>
/// �ז�����v���C���[���Ǘ�����N���X
/// </summary>
class AIPlayer : public PlayerBase
{
private:

	//���n���h�����f���ԍ�
	int hModel_;					//���f���ԍ�
	int hSound_[5];					//�T�E���h�ԍ�
	int stageHModel_;				//�X�e�[�W���f���ԍ�
	int floorHModel_;				//���蔲�������f���ԍ�
	int number_;
	int slowTime_;
	int slowTimeWait_;
	const int slowTimeNum_ = 2;
	const int defaultTimeNum_ = 1;
	int waitTime_;
	int waitTimeWait_;
	int attackTime_;
	int attackTimeWait_;
	bool isAttack_;
	bool isAttackFinish_;
	int attackSeeTime_;
	int attackSeeTimeWait_;
	bool isAttackSee_;
	bool isAttackSeeFinish_;
	int random_value_;
	XMVECTOR dir_;

	GAMESTATE gameState_;
	GameObject* pParent_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	Stage* pStage_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
	StateManager* pStateManager_;
	ImageManager* pImageManager_;
	ImageManager* pBoneImageManager_;
	ParticleManager* pParticleManager_;
	AIPlayerWaitSelector* pAIPlayerWaitSelector_;
public:
	enum class SOUNDSTATE
	{
		STUN = 0,
		WALK,
		JUMP,
		RUN,
		CollectBone,
	};

	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	AIPlayer(GameObject* _pParent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~AIPlayer();

	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() override;

	void UpdateSlow();

	/// <summary>
	/// �`��֐�
	/// </summary>
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;

	/// <summary>
	/// �J���֐�
	/// </summary>
	void Release() override;

	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();

	void PlayerAttackActionFunc();
	void PlayerAttackSeeActionFunc();
	void PlayerAttackDiveActionFunc();
	void PlayerWaitStateFunc();
	void PlayerWalkStateFunc();
	void PlayerRunStateFunc();
	void PlayerJumpStateFunc();
	void PlayerStunStateFunc();

	/// <summary>
	/// �����ɓ����������̊֐�
	/// </summary>
	/// <param name="_pTarget">������������</param>
	void OnCollision(GameObject* _pTarget) override;

	void PlayerAddScore() override;

	void PlayerCamera() override;

	void PlayerFall() override;

	/// <summary>
	/// �v���C���[�̈ړ��֐�
	/// </summary>
	void PlayerMove() override;

	void PlayerJump() override;

	/// <summary>
	/// �v���C���[�̃W�����v�֐�
	/// </summary>
	void PlayerJumpPower() override;

	void PlayerDive() override;

	void PlayerDivePower() override;

	void PlayerKnockback() override;

	/// <summary>
	/// �v���C���[�̃��C�֐�
	/// </summary>
	void PlayerRayCast() override;

	void PlayerRevival() override;

	/// <summary>
	/// �v���C���[���X�^��(�s���s�\)�ɂ��鏈��
	/// </summary>
	/// <param name="_timeLimit">_timeLimit�b�܂ŁA�������Ȃ�</param>
	void PlayerStun(int _timeLimit = 60) override;

	void PlayerOuterWall() override;

	void IsMove() override;

	void IsJump() override;

	void IsRun() override;

	void IsStun() override;

	void IsDive() override;

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	void SetCollectPlayer(CollectPlayer* _pCollectPlayer) { pCollectPlayer_ = _pCollectPlayer; }

	void SetAttackPlayer(AttackPlayer* _pAttackPlayer) { pAttackPlayer_ = _pAttackPlayer; }

	void SetKillTime(int _killTime) { boneData_.killTime_ = _killTime; }

	void SetImageSecInit();

	bool GetIsMove() override { return moveData_.isMove_; }

	bool GetIsJump() override { return jumpData_.isJump_; }

	bool GetIsRun() override { return moveData_.isRun_; }

	bool GetIsStun() override { return stunData_.isStun_; }

	bool GetIsDive() override { return diveData_.isDive_; }

	bool GetIsAttack() { return isAttack_; }

	bool GetIsAttackSee() { return isAttackSee_; }

	bool GetIsAttackSeeFinish() { return isAttackSeeFinish_; }

	bool GetIsBoneTatch() { return boneData_.isBoneTatch_; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	int GetPadID()override { return gameData_.padID_; }

	int GetModelHandle() override { return hModel_; }

	float GetAngle() { return dirData_.angle_; }

	int GetScore() { return gameData_.score_; }
};

