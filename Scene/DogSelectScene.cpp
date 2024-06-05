//インクルード
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
#include "../ImageManager.h"
#include "../UIManager.h"
#include "DogSelectScene.h"

DogSelectScene::DogSelectScene(GameObject* _pParent)
	:GameObject(_pParent, selectSceneName),attackOrCollect_{0},attackOrCollectInverse_{0},walkOrFight_{0}, solidTextRotate_{0.3f},padIDNum_{0}
	, skyPos_{0.0f,0.0f,0.0f}
	, pSceneManager_{nullptr}, pStageObjectManager_{nullptr},pSky_{nullptr}, pActorAttackPlayer_{ nullptr }
	, pActorCollectPlayer_{ nullptr },pImageManager_{nullptr},pUIManager_{nullptr},pDogsSelectUIManager_{nullptr}
{

}

void DogSelectScene::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
	ShowCursor();
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pSky_ = (Sky*)pStageObjectManager_->GetStageObjectBase();
	XMFLOAT3 positionStage = { 3.0f,38.0f,10.0f };
	pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::STAGE, positionStage);
	camPos_.y += 2;
	camPos_.z += 15;
	pActorAttackPlayer_ = Instantiate<ActorAttackPlayer>(this);
	pActorCollectPlayer_ = Instantiate<ActorCollectPlayer>(this);
	pActorAttackPlayer_->SetIsSelect(true);
	pActorCollectPlayer_->SetIsSelect(true);
	XMFLOAT3 positionActorAttackPlayer = { -5.0f,0.0f,0.0f };
	XMFLOAT3 positionActorCollectPlayer = { 5.0f,0.0f,0.0f };
	pActorAttackPlayer_->SetPosition(positionActorAttackPlayer);
	pActorCollectPlayer_->SetPosition(positionActorCollectPlayer);
	pImageManager_ = Instantiate<ImageManager>(this);
	pImageManager_->SetMode((int)IMAGESTATE::GAMETITLE);
	pUIManager_ = Instantiate<UIManager>(this);
	pUIManager_->SetMode((int)UISTATE::GAMEMANUAL);
	pDogsSelectUIManager_ = Instantiate<UIManager>(this);
	pDogsSelectUIManager_->SetMode((int)UISTATE::DOGSSELECT);
}

void DogSelectScene::Update()
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

	const float deadZone = 0.3f;
	if (Input::GetPadStickL((int)PADIDSTATE::FIRST).x < -deadZone)   //右への移動
	{
		attackOrCollect_ = (int)PADIDSTATE::SECONDS;
	}
	if (Input::GetPadStickL((int)PADIDSTATE::FIRST).x > deadZone)   //右への移動
	{
		attackOrCollect_ = (int)PADIDSTATE::FIRST;
	}
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::FIRST) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS))
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
		//▼INIファイルへの書き込み
		WritePrivateProfileString("PLAYERPADID", "AttackOrCollect", std::to_string(attackOrCollect_).c_str(), "Setting/PlayerSetting.ini");
		WritePrivateProfileString("PLAYERPADID", "AttackOrCollectInverse", std::to_string(attackOrCollectInverse_).c_str(), "Setting/PlayerSetting.ini");
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
		{
			pSceneManager_->ChangeScene(SCENE_ID_DOGS_WALK_PLAY);
		}
		else
		{
			pSceneManager_->ChangeScene(SCENE_ID_DOGS_FIGHT_PLAY);
		}
	}
	if (Input::IsKeyDown(DIK_Q) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, (int)PADIDSTATE::FIRST) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, (int)PADIDSTATE::SECONDS))
	{
		pSceneManager_->ChangeScene(SCENE_ID_GAMESELECT);
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
	static bool isViewManual = false;
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_X))
	{
		isViewManual = !isViewManual;
	}
	static bool isOnce = true;
	static bool isOncePrev = false;
	if (isViewManual)
	{
		if (isOnce != isOncePrev)
		{
			pImageManager_ = Instantiate<ImageManager>(this);
			isOncePrev = isOnce;
		}
		pImageManager_->SetMode((int)IMAGESTATE::GAMEMANUAL);
	}
	else
	{
		isOncePrev = false;
		pImageManager_->KillMe();

	}
}

void DogSelectScene::Draw()
{
}

void DogSelectScene::Release()
{

}
