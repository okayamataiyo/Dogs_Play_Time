//インクルード
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../ImageManager.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "GameTitleScene.h"

using enum ImageManager::IMAGESTATE;
using enum PADIDSTATE;
using enum TEXTSTATE;
using enum STAGEOBJECTSTATE;
using enum MOUSESTATE;
using enum Direct3D::VIEWSTATE;

GameTitleScene::GameTitleScene(GameObject* _pParent)
	:GameObject(_pParent, gameTitleSceneName), hSound_{-1},soundVolume_{0.2f},camPos_{0.0f,0.0f,0.0f}
	,pSolidText_{nullptr},pImageManager_{nullptr}, pStageObjectManager_{nullptr}, pSceneManager_{nullptr}
{

}

void GameTitleScene::Initialize()
{
	ShowCursor();
	//サウンドデータのロード
	std::string soundName = soundFolderName + soundGameTitleSceneName + soundModifierName;
	hSound_ = Audio::Load(soundName);
	assert(hSound_ >= 0);
	pSolidText_ = Instantiate<SolidText>(this);
	pSolidText_->SetMode((int)GAMETITLE);
	XMFLOAT3 positionStage = { 0.0f,38.0f,10.0f };
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(SKY);
	pStageObjectManager_->CreateStageObject(STAGE, positionStage);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	camTar_ = pSolidText_->GetPosition();
	camTar_.y += 2;
	pImageManager_ = Instantiate<ImageManager>(this);
	pImageManager_->SetMode((int)GAMETITLEIMAGE);
	pImageManager_->SecInit();
}

void GameTitleScene::Update()
{
	Audio::Play(hSound_, soundVolume_);


	Camera::SetPosition(camPos_, (int)SECONDS);
	Camera::SetTarget(camTar_, (int)SECONDS);
	Camera::SetPosition(camPos_, (int)FIRST);
	Camera::SetTarget(camTar_, (int)FIRST);
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B,(int)FIRST))
	{
		pSceneManager_->ChangeScene(SCENE_ID_GAMESELECT);
	}
	if (Input::IsKeyDown(DIK_R))
	{
		Direct3D::SetIsChangeView((int)RIGHT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_T))
	{
		Direct3D::SetIsChangeView((int)RIGHTVIEW);
	}
	if (Input::IsKeyDown(DIK_Y))
	{
		Direct3D::SetIsChangeView((int)LEFT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_U))
	{
		Direct3D::SetIsChangeView((int)LEFTVIEW);
	}
}

void GameTitleScene::BothViewDraw()
{
}

void GameTitleScene::LeftViewDraw()
{
}

void GameTitleScene::RightViewDraw()
{
}

void GameTitleScene::UPSubViewDraw()
{
}

void GameTitleScene::Release()
{
}
