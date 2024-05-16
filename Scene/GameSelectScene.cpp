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
#include "GameSelectScene.h"

GameSelectScene::GameSelectScene(GameObject* _pParent)
	:GameObject(_pParent,gameSelectSceneName), hPict_{-1},attackOrCollect_{0},attackOrCollectInverse_{0},padIDNum_{0}
	,skyPos_{0.0f,0.0f,0.0f},skyPosFly_{10000.0f,0.0f,10000.0f}
	, pSceneManager_{ nullptr }, pStageObjectManager_{ nullptr }, pSky_{ nullptr }
{
}

void GameSelectScene::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");

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
	XMFLOAT3 positionActorAttackPlayer = { 5.0f,0.0f,0.0f };
	XMFLOAT3 positionActorCollectPlayer = { -5.0f,0.0f,0.0f };
}

void GameSelectScene::Update()
{
	Camera::SetPosition(camPos_, attackOrCollect_);
	Camera::SetPosition(camPos_, attackOrCollectInverse_);

	const XMFLOAT3 bigScale = { 1.1f,1.1f,1.1f };
	const XMFLOAT3 defaultScale = { 0.8f,0.8f,0.8f };
}

void GameSelectScene::Draw()
{
}

void GameSelectScene::Release()
{
}
