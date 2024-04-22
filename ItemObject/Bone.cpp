//インクルード
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Scene/PlayScene.h"
#include "../StageObject/Stage.h"
#include "BoneSuck.h"
#include "Bone.h"

Bone::Bone(GameObject* _parent)
	:ItemObjectBase(_parent, boneName), hModel_{-1},rayDist_{0.0f},positionRotate_{1.0f}
	, boneInitPosY_{ 0.6f },pickUpBoneScale_{0.2f,0.2f,0.2f}
	,pPlayScene_{nullptr},pCollision_{nullptr},pStage_{nullptr},pCollectPlayer_{nullptr}
{
}

Bone::~Bone()
{

}

void Bone::Initialize()
{
	//モデルのロード
	std::string ModelName = modelFolderName + boneName + modelModifierName;
	hModel_ = Model::Load(ModelName);
	assert(hModel_ >= initZeroInt);

	pCollision_ = new SphereCollider(initZeroXMFLOAT3, 1.0f);
	AddCollider(pCollision_);
	pPlayScene_ = (PlayScene*)FindObject(playSceneName);
	pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
	transform_.scale_ = { 0.5,0.5,0.5 };
	transform_.position_ = { 10,0,0 };
}

void Bone::Update()
{
	transform_.rotate_.y += positionRotate_;
	RayCastData data;
	pStage_ = (Stage*)FindObject(stageName);    //ステージオブジェクト
	int stageHModel;
	stageHModel = pStage_->GetModelHandle();   //モデル番号を取得
	data.start = transform_.position_;  //レイの発射位置
	data.start.y = initZeroFloat;
	XMStoreFloat3(&data.dir, vecDown);	 //レイの方向
	Model::RayCast(stageHModel, &data);  //レイを発射
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
	pCollectPlayer_->SetIsBoneDeath(false);
}

void Bone::OnCollision(GameObject* _pTarget)
{
	if(_pTarget->GetObjectName() == collectPlayerName)
	{
		Instantiate<BoneSuck>(pPlayScene_);
		BoneDeath();
	}
}
