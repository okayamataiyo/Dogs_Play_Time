#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"

class SolidText;
class StageObjectManager;
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
	StageObjectManager* pStageObjectManager_;
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
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < initZeroInt); }
};

