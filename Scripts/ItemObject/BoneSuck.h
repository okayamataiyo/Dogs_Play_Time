#pragma once
//�C���N���[�h
#include "ItemObjectBase.h"

class Dogs_Walk_PlayScene;
class Dogs_Fight_PlayScene;
class SphereCollider;
class Stage;
class CollectPlayer;
class AttackPlayer;
class Bone;

namespace
{
	std::string boneSuckName = "Bone";
}

/// <summary>
/// �A�C�e�����Ǘ�����N���X
/// </summary>
class BoneSuck : public ItemObjectBase
{
protected:
	int hModel_;
	int playerHModel_;
	float rayDist_;
	float positionRotate_;
	int walkOrFight_;
	XMFLOAT3 playerRot_;
	int killTime_;
	int killTimeWait_;
	int killTimeMax_;
	XMFLOAT3 pickUpBoneSuckScale_;
	GameObject* pParent_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	Dogs_Fight_PlayScene* pDogs_Fight_PlayScene_;
	SphereCollider* pCollision_;
	Stage* pStage_;
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
	Bone* pBone_;
public:
	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	BoneSuck(GameObject* _parent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~BoneSuck();

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

	void PlayerSuckBone();
	
	/// <summary>
	/// �������W���̌��̌��ɂ��邽�߂̌v�Z
	/// </summary>
	void CalcPlayerFrontRot();

	void BoneSuckDeath();

	void SetKillTime(int _killTime) { killTime_ = _killTime; }

	int GetModelHandle() { return hModel_; }


};