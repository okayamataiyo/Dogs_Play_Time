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
BoneSuck::BoneSuck(GameObject* _parent)
	:ItemObjectBase(_parent, boneSuckName), hModel_{ -1 }, rayDist_{ 0.0f }, positionRotate_{ 1.0f }
	, BoneSuckInitPosY_{ 0.6f },killTime_{9999},killTimeWait_{30},killTimeMax_{9999}, pickUpBoneSuckScale_{0.2f,0.2f,0.2f}
	, pPlayScene_{ nullptr }, pCollision_{ nullptr }, pStage_{ nullptr }, pCollectPlayer_{ nullptr },pBone_{nullptr}
{
}

BoneSuck::~BoneSuck()
{
}

void BoneSuck::Initialize()
{
	//モデルのロード
	std::string ModelName = modelFolderName + boneSuckName + modelModifierName;
	hModel_ = Model::Load(ModelName);
	assert(hModel_ >= initZeroInt);
	pPlayScene_ = (PlayScene*)FindObject(playSceneName);
	pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
	pBone_ = (Bone*)FindObject(boneName);
	transform_.scale_ = { 0.5,0.5,0.5 };
	transform_.position_ = { 10,0,0 };
	SetKillTime(killTimeWait_);
}

void BoneSuck::Update()
{
	transform_.rotate_.y += positionRotate_;
	PlayerSuckBoneSuck();
	if (killTime_ > initZeroInt)
	{
		--killTime_;
	}

	if (killTime_ <= initZeroInt)
	{
		killTime_ = killTimeMax_;
		BoneSuckDeath();
	}
}

void BoneSuck::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void BoneSuck::Release()
{
}

void BoneSuck::PlayerSuckBoneSuck()
{
	XMFLOAT3 BoneSuckPosition_ = Model::GetBonePosition(pCollectPlayer_->GetModelHandle(), "joint3");
	transform_.position_ = BoneSuckPosition_;
	transform_.rotate_.y = XMConvertToDegrees(pCollectPlayer_->GetAngle());
	transform_.scale_ = pickUpBoneSuckScale_;
}

void BoneSuck::BoneSuckDeath()
{
	this->KillMe();
}