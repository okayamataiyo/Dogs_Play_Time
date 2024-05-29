//インクルード
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../Engine/ImGui/imgui.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Player/AttackPlayer.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "../StageObject/Stage.h"
#include "BoneSuck.h"
#include "Bone.h"
BoneSuck::BoneSuck(GameObject* _parent)
	:ItemObjectBase(_parent, boneSuckName), hModel_{-1},playerHModel_{-1}, rayDist_{0.0f}, positionRotate_{1.0f}
	,walkOrFight_{0},playerRot_{}, killTime_{9999}, killTimeWait_{30}, killTimeMax_{9999}, pickUpBoneSuckScale_{0.2f,0.2f,0.2f}
	, pParent_{ _parent }, pDogs_Walk_PlayScene_{ nullptr },pDogs_Fight_PlayScene_{nullptr}, pCollision_{nullptr}, pStage_{nullptr}, pCollectPlayer_{nullptr}
	,pAttackPlayer_{nullptr}, pBone_{nullptr}
{
}

BoneSuck::~BoneSuck()
{
}

void BoneSuck::Initialize()
{
	//INIファイルからロード
	walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
	//モデルのロード
	std::string ModelName = modelFolderName + boneSuckName + modelModifierName;
	hModel_ = Model::Load(ModelName);
	assert(hModel_ >= 0);
	pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
	pDogs_Fight_PlayScene_ = (Dogs_Fight_PlayScene*)FindObject(Dogs_Fight_PlaySceneName);
	pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
	pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
	pBone_ = (Bone*)FindObject(boneName);
	transform_.scale_ = { 0.5,0.5,0.5 };
	transform_.position_ = { 0,2,5 };
	SetKillTime(killTimeWait_);
}

void BoneSuck::Update()
{
	PlayerSuckBone();
	if (killTime_ > 0)
	{
		--killTime_;
	}

	if (killTime_ <= 0)
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

void BoneSuck::PlayerSuckBone()
{
	if (pParent_->GetObjectName() == collectPlayerName)
	{
		playerHModel_ = pCollectPlayer_->GetModelHandle();
		transform_.rotate_.y = playerRot_.y;
	}
	if (pParent_->GetObjectName() == attackPlayerName)
	{
		playerHModel_ = pAttackPlayer_->GetModelHandle();
		transform_.rotate_.y = playerRot_.y;
	}
}

void BoneSuck::CalcPlayerFrontRot()
{
	if (pParent_->GetObjectName() == collectPlayerName)
	{
		playerRot_ = pCollectPlayer_->GetPosition();
		static XMVECTOR playerDir = XMLoadFloat3(&playerRot_) - Camera::VecGetPosition(pCollectPlayer_->GetPadID());
		static float boneFrontPosition = 2.0f;
		playerDir = XMVectorSetY(playerDir, 0);	//y座標の初期化
		playerDir = XMVector3Normalize(playerDir);
		playerRot_.x += boneFrontPosition * XMVectorGetX(playerDir);
		playerRot_.z += boneFrontPosition * XMVectorGetZ(playerDir);
	}
	if (pParent_->GetObjectName() == attackPlayerName)
	{
		playerRot_ = pAttackPlayer_->GetPosition();
		static XMVECTOR playerDir = XMLoadFloat3(&playerRot_) - Camera::VecGetPosition(pAttackPlayer_->GetPadID());
		static float boneFrontPosition = 2.0f;
		playerDir = XMVectorSetY(playerDir, 0);	//y座標の初期化
		playerDir = XMVector3Normalize(playerDir);
		playerRot_.x += boneFrontPosition * XMVectorGetX(playerDir);
		playerRot_.z += boneFrontPosition * XMVectorGetZ(playerDir);
	}
}

void BoneSuck::BoneSuckDeath()
{
	this->KillMe();
}