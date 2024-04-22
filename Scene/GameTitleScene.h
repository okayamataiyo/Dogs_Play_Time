#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"

namespace
{
	std::string soundGameTitleSceneName = "TitleBGM";
	std::string gameTitleSceneName = "GameTitleScene";
}

class StageObjectManager;
class SolidText;
class SceneManager;

class GameTitleScene : public GameObject
{
private:
	int hSound_;
	float soundVolume_;
	XMFLOAT3 camPos_;
	SolidText* pSolidText_;
	StageObjectManager* pStageObjectManager_;
	SceneManager* pSceneManager_;
public:
	GameTitleScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < 0); }
};

