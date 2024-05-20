//�C���N���[�h
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Global.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../StageObject/Sky.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "../Player/ActorAttackPlayer.h"
#include "../Player/ActorCollectPlayer.h"
#include "SelectScene.h"

SelectScene::SelectScene(GameObject* _pParent)
	:GameObject(_pParent, selectSceneName), hPict_{ -1 },attackOrCollect_{0},attackOrCollectInverse_{0}, solidTextRotate_{0.3f}, isViewPicture_{false}, padIDNum_{0}
	, skyPos_{0.0f,0.0f,0.0f}, skyPosFly_{10000.0f,0.0f,10000.0f}
	,pSceneManager_{nullptr}, pStageObjectManager_{nullptr},pSky_{nullptr}, pActorAttackPlayer_{ nullptr }, pActorCollectPlayer_{ nullptr }
{

}

void SelectScene::Initialize()
{
	//��INI�t�@�C������f�[�^�̃��[�h
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");

	//���摜�f�[�^�̃��[�h
	std::string imageName = modelFolderName + manualName + imageModifierName;
	hPict_ = Image::Load(imageName);
	assert(hPict_ >= initZeroInt);
	pictureTrans_ = {};
	ShowCursor();
	//pSolidText_ = Instantiate<SolidText>(this);
	//pSolidText_->SetMode((int)TEXTSTATE::SELECT);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pSky_ = (Sky*)pStageObjectManager_->GetStageObjectBase();
	//camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z += 15;
	pActorAttackPlayer_ = Instantiate<ActorAttackPlayer>(this);
	pActorCollectPlayer_ = Instantiate<ActorCollectPlayer>(this);
	pActorAttackPlayer_->SetIsSelect(true);
	pActorCollectPlayer_->SetIsSelect(true);
	XMFLOAT3 positionActorAttackPlayer = { 5.0f,0.0f,0.0f };
	XMFLOAT3 positionActorCollectPlayer = { -5.0f,0.0f,0.0f };
	pActorAttackPlayer_->SetPosition(positionActorAttackPlayer);
	pActorCollectPlayer_->SetPosition(positionActorCollectPlayer);
}

void SelectScene::Update()
{
	Camera::SetPosition(camPos_, attackOrCollect_);
	Camera::SetPosition(camPos_, attackOrCollectInverse_);

	const XMFLOAT3 bigScale = { 1.1f,1.1f,1.1f };
	const XMFLOAT3 defaultScale = { 0.8f,0.8f,0.8f };

	if (attackOrCollect_ == (int)PADIDSTATE::FIRST)
	{
		pActorAttackPlayer_->SetScale(bigScale);
		pActorCollectPlayer_->SetScale(defaultScale);
	}
	else
	{
		pActorAttackPlayer_->SetScale(defaultScale);
		pActorCollectPlayer_->SetScale(bigScale);
	}

	XMFLOAT3 pos = Input::GetMousePosition();
	const float deadZone = 0.3f;
	switch (padIDNum_)
	{
	case (int)PADIDSTATE::FIRST:
		if (Input::GetPadStickL((int)PADIDSTATE::FIRST).x < -deadZone)   //�E�ւ̈ړ�
		{
			attackOrCollect_ = (int)PADIDSTATE::FIRST;
		}
		if (Input::GetPadStickL((int)PADIDSTATE::FIRST).x > deadZone)   //�E�ւ̈ړ�
		{
			attackOrCollect_ = (int)PADIDSTATE::SECONDS;
		}
		break;
	case (int)PADIDSTATE::SECONDS:
		break;
	}
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A, (int)PADIDSTATE::FIRST))
	{
		//padID_ = (int)PADIDSTATE::FIRST;
	}

	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A, (int)PADIDSTATE::SECONDS))
	{
		//padID_ = (int)PADIDSTATE::SECONDS;
	}

	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, (int)PADIDSTATE::FIRST) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, (int)PADIDSTATE::SECONDS))
	{
		if (Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::LEFTVIEW)
		{
			Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW);
		}
		if (Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::RIGHTVIEW)
		{
			Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW);
		}
		if (attackOrCollect_ == (int)PADIDSTATE::FIRST)
		{
			attackOrCollectInverse_ = (int)PADIDSTATE::SECONDS;
		}
		else
		{
			attackOrCollectInverse_ = (int)PADIDSTATE::FIRST;
		}
		//��INI�t�@�C���ւ̏�������
		WritePrivateProfileString("PLAYERPADID", "AttackOrCollect", std::to_string(attackOrCollect_).c_str(), "Setting/PlayerSetting.ini");
		WritePrivateProfileString("PLAYERPADID", "AttackOrCollectInverse", std::to_string(attackOrCollectInverse_).c_str(), "Setting/PlayerSetting.ini");
		pSceneManager_->ChangeScene(SCENE_ID_DOGS_FIGHT_PLAY);
	}
	if (Input::IsKeyDown(DIK_R))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_T))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHTVIEW);
	}
	if (Input::IsKeyDown(DIK_Y))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_U))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFTVIEW);
	}

	if (Input::IsMouseButtonDown((int)MOUSESTATE::RIGHTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_X, attackOrCollect_) || Input::IsPadButtonDown(XINPUT_GAMEPAD_X, attackOrCollectInverse_))
	{
		isViewPicture_ = !isViewPicture_;
	}
	if (isViewPicture_)
	{
		pSky_->SetPosition(skyPosFly_);
	}
	else
	{
		pSky_->SetPosition(skyPos_);
	}

}

void SelectScene::Draw()
{
	Image::SetTransform(hPict_, pictureTrans_);
	Image::Draw(hPict_);
}

void SelectScene::Release()
{

}
