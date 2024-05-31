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
#include "../UI.h"
#include "../GameImage.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(GameObject* _pParent)
	:GameObject(_pParent, gameOverSceneName), hSound_{ -1 }, soundVolume_{0.1f},inputWait_{ 0 }
	, inputWaitTime_{ 60 },camPos_{0.0f,0.0f,0.0f},camTar_{0.0f,0.0f,0.0f}
	,pSolidText_{nullptr}, pStageObjectManager_{nullptr}, pSceneManager_{nullptr},pStage_{nullptr}
	,pUI_{nullptr},pGameImage_{nullptr}, pActorAttackPlayer_{nullptr}, pActorCollectPlayer_{nullptr}
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
	pSolidText_->SetMode((int)(TEXTSTATE::GAMEOVER));
	XMFLOAT3 positionStage = { 0.0f,38.0f,10.0f };
	XMFLOAT3 positionActorAttackPlayer = { 0.0f,0.0f,20.0f };
	XMFLOAT3 positionActorCollectPlayer = { 0.0f,0.0f,25.0f };
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::STAGE, positionStage);
	pStage_ = (Stage*)pStageObjectManager_->GetStageObjectBase();
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	camTar_ = pSolidText_->GetPosition();
	camTar_.y += 2;
	pActorAttackPlayer_ = Instantiate<ActorAttackPlayer>(this);
	pActorCollectPlayer_ = Instantiate<ActorCollectPlayer>(this);
	pActorAttackPlayer_->SetPosition(positionActorAttackPlayer);
	pActorCollectPlayer_->SetPosition(positionActorCollectPlayer);
	pUI_ = Instantiate<UI>(this);
	pGameImage_ = Instantiate<GameImage>(this);
}

void GameOverScene::Update()
{
	Audio::Play(hSound_, soundVolume_);
	Camera::SetPosition(camPos_, (int)PADIDSTATE::SECONDS);
	Camera::SetTarget(camTar_, (int)PADIDSTATE::SECONDS);
	Camera::SetPosition(camPos_, (int)PADIDSTATE::FIRST);
	Camera::SetTarget(camTar_, (int)PADIDSTATE::FIRST);
	++inputWait_;
	if (inputWait_ >= inputWaitTime_)
	{
		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::FIRST))
		{
			pSceneManager_->ChangeScene(SCENE_ID_GAMETITLE);
			inputWait_ = initZeroInt;
		}
	}
}

void GameOverScene::Draw()
{
	
}

void GameOverScene::Release()
{
}
