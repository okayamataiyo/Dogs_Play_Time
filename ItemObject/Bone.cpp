//�C���N���[�h
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../StageObject/Stage.h"
#include "BoneSuck.h"
#include "Bone.h"

Bone::Bone(GameObject* _parent)
	:ItemObjectBase(_parent, boneName), hModel_{-1},rayDist_{0.0f},positionRotate_{1.0f}
	, boneInitPosY_{ 1.0f }, pickUpBoneScale_{0.2f,0.2f,0.2f}
	,pDogs_Walk_PlayScene_{nullptr},pCollision_{nullptr},pStage_{nullptr},pCollectPlayer_{nullptr}
{
}

Bone::~Bone()
{

}

void Bone::Initialize()
{
	//���f���̃��[�h
	std::string ModelName = modelFolderName + boneName + modelModifierName;
	hModel_ = Model::Load(ModelName);
	assert(hModel_ >= initZeroInt);

	pCollision_ = new SphereCollider(initZeroXMFLOAT3, 1.0f);
	AddCollider(pCollision_);
	pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
	pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
	transform_.scale_ = { 0.7f,0.7f,0.7f };
	transform_.position_ = { 10.0,0.0,0.0 };
}

void Bone::Update()
{
	transform_.rotate_.y += positionRotate_;
	RayCastData data;
	pStage_ = (Stage*)FindObject(stageName);    //�X�e�[�W�I�u�W�F�N�g
	int stageHModel;
	stageHModel = pStage_->GetModelHandle();   //���f���ԍ����擾
	data.start = transform_.position_;  //���C�̔��ˈʒu
	data.start.y = initZeroFloat;
	XMStoreFloat3(&data.dir, vecDown);	 //���C�̕���
	Model::RayCast(stageHModel, &data);  //���C�𔭎�
	rayDist_ = data.dist;

	if (data.hit)
	{
		transform_.position_.y = -data.dist + boneInitPosY_;
	}
}

void Bone::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bone::Release()
{
}
void Bone::BoneDeath()
{
	this->KillMe();
}

void Bone::OnCollision(GameObject* _pTarget)
{
	if(_pTarget->GetObjectName() == collectPlayerName)
	{

	}
}
