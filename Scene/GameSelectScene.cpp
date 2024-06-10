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
#include "GameSelectScene.h"

GameSelectScene::GameSelectScene(GameObject* _pParent)
	:GameObject(_pParent, gameSelectSceneName), hPict_{ -1 }, attackOrCollect_{ 0 }, attackOrCollectInverse_{ 0 },walkOrFight_{0}, padIDNum_{0}
	, skyPos_{ 0.0f,0.0f,0.0f }, skyPosFly_{ 10000.0f,0.0f,10000.0f }
	, pSceneManager_{ nullptr }, pStageObjectManager_{ nullptr }, pSky_{ nullptr }
	, pDogsWalkText_{ nullptr }, pDogsFightText_{ nullptr }, pImageManager_{ nullptr }, pDogsSelectUIManager_{ nullptr }
{
}

void GameSelectScene::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
	ShowCursor();
	pDogsWalkText_ = Instantiate<SolidText>(this);
	pDogsWalkText_->SetMode((int)TEXTSTATE::DOGS_WALK);
	pDogsWalkText_->SetRotateY(0.0f);
	pDogsWalkText_->SetPosition(XMFLOAT3(5.0f, 0.0f, 0.0f));
	pDogsFightText_ = Instantiate<SolidText>(this);
	pDogsFightText_->SetMode((int)TEXTSTATE::DOGS_FIGHT);
	pDogsFightText_->SetRotateY(0.0f);
	pDogsFightText_->SetPosition(XMFLOAT3(-5.0f, 0.0f, 0.0f));
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pSky_ = (Sky*)pStageObjectManager_->GetStageObjectBase();
	XMFLOAT3 positionStage = { 5.0f,38.0f,10.0f };
	pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::STAGE, positionStage);
	camPos_.y += 2;
	camPos_.z += 15;
	XMFLOAT3 positionActorAttackPlayer = { 5.0f,0.0f,0.0f };
	XMFLOAT3 positionActorCollectPlayer = { -5.0f,0.0f,0.0f };
	pImageManager_ = Instantiate<ImageManager>(this);
	pImageManager_->SetMode((int)IMAGESTATE::GAMETITLE);
	pDogsSelectUIManager_ = Instantiate<UIManager>(this);
	pDogsSelectUIManager_->SetMode((int)UISTATE::DOGSSELECT);
}

void GameSelectScene::Update()
{
	Camera::SetPosition(camPos_, attackOrCollect_);
	Camera::SetPosition(camPos_, attackOrCollectInverse_);

	const XMFLOAT3 bigScale = { 0.6f,0.6f,0.6f };
	const XMFLOAT3 defaultScale = { 0.5f,0.5f,0.5f };

	if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
	{
		pDogsWalkText_->SetScale(bigScale);
		pDogsFightText_->SetScale(defaultScale);
	}
	else
	{
		pDogsWalkText_->SetScale(defaultScale);
		pDogsFightText_->SetScale(bigScale);
	}

	const float deadZone = 0.3f;
	if (Input::GetPadStickL((int)PADIDSTATE::FIRST).x < -deadZone || Input::GetPadStickL((int)PADIDSTATE::SECONDS).x < -deadZone)
	{
		walkOrFight_ = (int)PLAYSCENESTATE::DOGSWALK;
	}
	if (Input::GetPadStickL((int)PADIDSTATE::FIRST).x > deadZone || Input::GetPadStickL((int)PADIDSTATE::SECONDS).x > deadZone)
	{
		walkOrFight_ = (int)PLAYSCENESTATE::DOGSFIGHT;
	}
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::FIRST) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS))
	{
		//INIファイルへの書き込み
		WritePrivateProfileString("PLAYSCENEID", "WalkOrFight", std::to_string(walkOrFight_).c_str(), "Setting/PlaySceneSetting.ini");
		pSceneManager_->ChangeScene(SCENE_ID_SELECT);
	}
}

void GameSelectScene::BothViewDraw()
{
}

void GameSelectScene::LeftViewDraw()
{

}

void GameSelectScene::RightViewDraw()
{
}

void GameSelectScene::UPSubViewDraw()
{
}

void GameSelectScene::Release()
{
}
