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
	,skyPos_{0.0f,0.0f,0.0f},skyPosFly_{10000}
{
}

void GameSelectScene::Initialize()
{
}

void GameSelectScene::Update()
{
}

void GameSelectScene::Draw()
{
}

void GameSelectScene::Release()
{
}
