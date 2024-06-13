#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;
class AttackPlayer;
class SphereCollider;
class WoodBox;
class BoneSuck;
class Text;
class Stage;
class StageBlock;
class Floor;
class SceneManager;
class ItemObjectManager;
class StateManager;
class ImageManager;
class ParticleManager;
class UIManager;

namespace
{
	std::string soundCollectPlayerNames[] =
	{
		"Stun",
		"Walk",
		"Jump",
		"Run",
		"CollectBone",
	};

	std::string collectPlayerName = "CollectPlayer";
}

/// <summary>
/// ���W����v���C���[���Ǘ�����N���X
/// </summary>
class CollectPlayer : public PlayerBase
{
private:

	enum class SOUNDSTATE
	{
		STUN = 0,
		WALK,
		JUMP,
		RUN,
		CollectBone,
	};

	//���n���h�����f���ԍ�
	int hModel_;					//���f���ԍ�
	int hSound_[5];					//�T�E���h�ԍ�
	int stageBlockHModel_;			//�X�e�[�W�u���b�N���f���ԍ�
	int stageHModel_;				//�X�e�[�W���f���ԍ�
	int floorHModel_;				//���蔲����
	int number_;
	int attackOrCollectInverse_;

	GAMESTATE gameState_;
	GameObject* pParent_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
	AttackPlayer* pAttackPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	BoneSuck* pBoneSuck_;
	Text* pText_;
	Stage* pStage_;
	StageBlock* pStageBlock_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
	StateManager* pStateManager_;
	ImageManager* pImageManager_;
	ImageManager* pBoneImageManager_;
	ParticleManager* pParticleManager_;
	UIManager* pUIManager_;
public:

	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	CollectPlayer(GameObject* _pParent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~CollectPlayer();

	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() override;

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

	void PlayerScore() override;

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

	void IsMove() override;

	void IsJump() override;

	void IsRun() override;

	void IsStun() override;

	void IsDive() override;

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	void SetAttackPlayer(AttackPlayer* _pAttackPlayer) { pAttackPlayer_ = _pAttackPlayer; }

	void SetKillTime(int _killTime) { boneData_.killTime_ = _killTime; }

	bool GetIsMove() override { return moveData_.isMove_; }

	bool GetIsJump() override { return jumpData_.isJump_; }

	bool GetIsRun() override { return moveData_.isRun_; }

	bool GetIsStun() override { return stunData_.isStun_; }

	bool GetIsDive() override { return diveData_.isDive_; }

	bool GetIsBoneTatch() {return boneData_.isBoneTatch_; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	int GetPadID()override { return gameData_.padID_; }

	int GetModelHandle() override { return hModel_; }

	float GetAngle() { return dirData_.angle_; }

	int GetScore() { return gameData_.score_; }
};

