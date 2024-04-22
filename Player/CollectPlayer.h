#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class PlayScene;
class AttackPlayer;
class SphereCollider;
class WoodBox;
class Text;
class Stage;
class StageBlock;
class Floor;
class SceneManager;
class ItemObjectManager;

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
	int collectPlayerNumber = 0;
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
	int decBoneCount_;
	bool isBoneDeath_;				//����������
	bool isBoneTatch_;
	int number_;
	int killTime_;
	int killTimeWait_;
	int killTimeMax_;
	XMVECTOR vecKnockbackDirection_;
	PLAYERSTATE playerState_;
	PLAYERSTATE playerStatePrev_;
	GAMESTATE gameState_;
	GameObject* pParent_;
	PlayScene* pPlayScene_;
	AttackPlayer* pAttackPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	Text* pText_;
	Stage* pStage_;
	StageBlock* pStageBlock_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
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

	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();
	/// <summary>
	/// �v���C���[���X�^��(�s���s�\)�ɂ��鏈��
	/// </summary>
	/// <param name="_timeLimit">_timeLimit�b�܂ŁA�������Ȃ�</param>
	void Stun(int _timeLimit = 60) override;

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �J���֐�
	/// </summary>
	void Release() override;

	/// <summary>
	/// �����ɓ����������̊֐�
	/// </summary>
	/// <param name="_pTarget">������������</param>
	void OnCollision(GameObject* _pTarget) override;

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

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	void SetAttackPlayer(AttackPlayer* _pAttackPlayer) { pAttackPlayer_ = _pAttackPlayer; }

	void SetIsBoneDeath(bool _isBoneDeath) { isBoneDeath_ = _isBoneDeath; }

	void SetKillTime(int _killTime) { killTime_ = _killTime; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	PLAYERSTATE GetPlayerState() override { return playerState_; }

	int GetPadID()override { return padID_; }

	int GetModelHandle() override { return hModel_; }

	bool GetIsBoneDeath() { return isBoneDeath_; }

	float GetAngle() { return angle_; }

	int GetScore() { return score_; }

	bool GetIsBoneTatch() { return isBoneTatch_; }

	bool IsMoving() override;
};

