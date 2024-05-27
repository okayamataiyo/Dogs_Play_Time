//インクルード
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Player/AttackPlayer.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../StageObject/Stage.h"
#include "BoneSuck.h"
#include "Bone.h"
BoneSuck::BoneSuck(GameObject* _parent)
	:ItemObjectBase(_parent, boneSuckName),pParent_{_parent}, hModel_{-1},playerHModel_{-1}, rayDist_{0.0f}, positionRotate_{1.0f}
	, BoneSuckInitPosY_{ 0.6f },killTime_{9999},killTimeWait_{30},killTimeMax_{9999}, pickUpBoneSuckScale_{0.2f,0.2f,0.2f}
	, pDogs_Walk_PlayScene_{ nullptr }, pCollision_{ nullptr }, pStage_{ nullptr }, pCollectPlayer_{ nullptr }
	,pAttackPlayer_{nullptr}, pBone_{nullptr}
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
	pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
	pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
	pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
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
	if (pParent_->GetObjectName() == collectPlayerName)
	{
		playerHModel_ = pCollectPlayer_->GetModelHandle();
		transform_.rotate_.y = pCollectPlayer_->GetAngle();
	}
	if (pParent_->GetObjectName() == attackPlayerName)
	{
		playerHModel_ = pAttackPlayer_->GetModelHandle();
		transform_.rotate_.y = pAttackPlayer_->GetAngle();
	}
	XMFLOAT3 BoneSuckPosition_ = Model::GetBonePosition(playerHModel_, "joint3");
	transform_.position_ = BoneSuckPosition_;
	transform_.scale_ = pickUpBoneSuckScale_;
}

void BoneSuck::BoneSuckDeath()
{
	this->KillMe();
}