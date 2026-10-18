#pragma once
//インクルード
#include "../../Engine/GameObject.h"
#include "../../Engine/Camera.h"
#include "../StageObject/StageObjectManager.h"

class SolidText;
class SceneManager;
class Stage;
class ActorAttackPlayer;
class ActorCollectPlayer;
class UIManager;
class ImageManager;

namespace
{
	std::string soundGameOverSceneNmae = "GameOverBGM";
	std::string gameOverSceneName = "GameOverScene";
}

class GameOverScene : public GameObject
{
private:
	int hSound_;
	float soundVolume_;
	int inputWait_;
	int inputWaitTime_;
	XMFLOAT3 camPos_;
	XMFLOAT3 camTar_;
	SolidText* pSolidText_;
	std::unique_ptr<StageObjectManager> pStageObjectManager_;	//このシーンが持つ
	SceneManager* pSceneManager_;
	Stage* pStage_;
	UIManager* pUIManager_;
	ImageManager* pImageManager_;
	ImageManager* pButtonImage_;
	ActorAttackPlayer* pActorAttackPlayer_;
	ActorCollectPlayer* pActorCollectPlayer_;
public:
	GameOverScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < initZeroInt); }
};

