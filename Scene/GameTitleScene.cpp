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
	pSolidText_->SetMode((int)TEXTSTATE::GAMETITLE);
	XMFLOAT3 positionStage = { 0.0f,38.0f,10.0f };
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::STAGE, positionStage);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	camTar_ = pSolidText_->GetPosition();
	camTar_.y += 2;
	pImageManager_ = Instantiate<ImageManager>(this);
	pImageManager_->SetMode((int)IMAGESTATE::GAMETITLE);
}

void GameTitleScene::Update()
{
	Audio::Play(hSound_, soundVolume_);


	Camera::SetPosition(camPos_, (int)PADIDSTATE::SECONDS);
	Camera::SetTarget(camTar_, (int)PADIDSTATE::SECONDS);
	Camera::SetPosition(camPos_, (int)PADIDSTATE::FIRST);
	Camera::SetTarget(camTar_, (int)PADIDSTATE::FIRST);
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B,(int)PADIDSTATE::FIRST))
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

void GameTitleScene::Release()
{
}
