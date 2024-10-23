#pragma once
//�C���N���[�h
#include "ItemObjectBase.h"

class Dogs_Walk_PlayScene;
class SphereCollider;
class Stage;
class CollectPlayer;

namespace
{
	std::string boneName = "Bone";
}

/// <summary>
/// �A�C�e�����Ǘ�����N���X
/// </summary>
class Bone : public ItemObjectBase
{
protected:
	int hModel_;
	float rayDist_;
	float positionRotate_;
	float boneInitPosY_;
	XMFLOAT3 pickUpBoneScale_;
	Dogs_Walk_PlayScene* pDogs_Walk_PlayScene_;
	SphereCollider* pCollision_;
	Stage* pStage_;
	CollectPlayer* pCollectPlayer_;
public:
	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	Bone(GameObject* _parent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~Bone();

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

	void BoneDeath();

	int GetModelHandle() { return hModel_; }

	/// <summary>
	/// �����ɓ����������̊֐�
	/// </summary>
	/// <param name="_pTarget">������������</param>
	void OnCollision(GameObject* _pTarget) override;


};