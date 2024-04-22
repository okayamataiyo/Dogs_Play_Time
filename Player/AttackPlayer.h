#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class PlayScene;
class CollectPlayer;
class SphereCollider;
class WoodBox;
class Text;
class Stage;
class Floor;
class SceneManager;
class ItemObjectManager;

namespace
{
	std::string soundAttackPlayerNames[] =
	{
		"Stun",
		"Walk",
		"Jump",
		"Run",
	};

	std::string attackPlayerName = "AttackPlayer";
	int attackPlayerNumber = 1;
}

/// <summary>
/// �ז�����v���C���[���Ǘ�����N���X
/// </summary>
class AttackPlayer : public PlayerBase
{
private:

	enum class SOUNDSTATE
	{
		STUN = 0,
		WALK,
		JUMP,
		RUN,
	};

	//���n���h�����f���ԍ�
	int hModel_;					//���f���ԍ�
	int hSound_[4];					//�T�E���h�ԍ�
	int stageHModel_;				//�X�e�[�W���f���ԍ�
	int floorHModel_;				//���蔲�������f���ԍ�
	int number_;
	int scoreTimeCounter_;
	int scoreTimeCounterWait_;
	XMVECTOR vecKnockbackDirection_;
	PLAYERSTATE playerState_;
	PLAYERSTATE playerStatePrev_;
	GAMESTATE gameState_;
	GameObject* pParent_;
	PlayScene* pPlayScene_;
	CollectPlayer* pCollectPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	Text* pText_;
	Stage* pStage_;
	Floor* pFloor_;
	SceneManager* pSceneManager_;
	ItemObjectManager* pItemObjectManager_;
public:

	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	AttackPlayer(GameObject* _pParent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~AttackPlayer();

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

	void SetCollectPlayer(CollectPlayer* _pCollectPlayer) { pCollectPlayer_ = _pCollectPlayer; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	PLAYERSTATE GetPlayerState() override { return playerState_; }

	float GetAngle() { return angle_; }

	int GetPadID()override { return padID_; }

	int GetScore() { return score_; }

	int GetModelHandle() override { return hModel_; };

	int GetIsJump() { return isJump_; }

	int GetIsStun() { return isStun_; }

	bool IsMoving() override;
};

