//インクルード
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../StageObject/Stage.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "../Player/ActorAttackPlayer.h"
#include "../Player/ActorCollectPlayer.h"
#include "../UIManager.h"
#include "../ImageManager.h"
#include "GameOverScene.h"

using enum UIManager::UISTATE;
using enum STAGEOBJECTSTATE;
using enum TEXTSTATE;
using enum ImageManager::IMAGESTATE;
using enum PADIDSTATE;
using enum MOUSESTATE;

GameOverScene::GameOverScene(GameObject* _pParent)
	:GameObject(_pParent, gameOverSceneName), hSound_{ -1 }, soundVolume_{0.1f},inputWait_{ 0 }
	, inputWaitTime_{ 60 },camPos_{0.0f,0.0f,0.0f},camTar_{0.0f,0.0f,0.0f}
	,pSolidText_{nullptr}, pStageObjectManager_{nullptr}, pSceneManager_{nullptr},pStage_{nullptr}
	,pUIManager_{nullptr},pImageManager_{nullptr},pButtonImage_{nullptr}, pActorAttackPlayer_{nullptr}, pActorCollectPlayer_{nullptr}
{

}

void GameOverScene::Initialize()
{
	ShowCursor();
	//サウンドデータのロード
	std::string soundName = soundFolderName + soundGameOverSceneNmae + soundModifierName;
	hSound_ = Audio::Load(soundName);
	assert(hSound_ >= initZeroInt);
	//画像データのロード
	pSolidText_ = Instantiate<SolidText>(this);
	pSolidText_->SetMode((int)(GAMEOVER));
	XMFLOAT3 positionStage = { 0.0f,38.0f,10.0f };
	XMFLOAT3 positionActorAttackPlayer = { 0.0f,0.0f,20.0f };
	XMFLOAT3 positionActorCollectPlayer = { 0.0f,0.0f,25.0f };
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(SKY);
	pStageObjectManager_->CreateStageObject(STAGE, positionStage);
	pStage_ = (Stage*)pStageObjectManager_->GetStageObjectBase();
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	camTar_ = pSolidText_->GetPosition();
	camTar_.y += 2;
	//pActorAttackPlayer_ = Instantiate<ActorAttackPlayer>(this);
	//pActorCollectPlayer_ = Instantiate<ActorCollectPlayer>(this);
	//pActorAttackPlayer_->SetPosition(positionActorAttackPlayer);
	//pActorCollectPlayer_->SetPosition(positionActorCollectPlayer);
	pUIManager_ = Instantiate<UIManager>(this);
	pUIManager_->SetMode((int)GAMEOVERUI);
	pImageManager_ = Instantiate<ImageManager>(this);
	pImageManager_->SetMode((int)GAMEOVERIMAGE);
	pImageManager_->SecInit();
	pButtonImage_ = Instantiate<ImageManager>(this);
	pButtonImage_->SetMode((int)GAMETITLEIMAGE);
	pButtonImage_->SecInit();
}

void GameOverScene::Update()
{
	Audio::Play(hSound_, soundVolume_);
	Camera::SetPosition(camPos_, (int)SECONDS);
	Camera::SetTarget(camTar_, (int)SECONDS);
	Camera::SetPosition(camPos_, (int)FIRST);
	Camera::SetTarget(camTar_, (int)FIRST);
	++inputWait_;
	if (inputWait_ >= inputWaitTime_)
	{
		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)FIRST))
		{
			pSceneManager_->ChangeScene(SCENE_ID_GAMETITLE);
			inputWait_ = initZeroInt;
		}
	}
}

void GameOverScene::BothViewDraw()
{
	
}

void GameOverScene::LeftViewDraw()
{
}

void GameOverScene::RightViewDraw()
{

}

void GameOverScene::UPSubViewDraw()
{
}

void GameOverScene::Release()
{
}
