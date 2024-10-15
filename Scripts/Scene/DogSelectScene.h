#pragma once
//インクルード
#include "../../Engine/GameObject.h"
#include "../../Engine/Camera.h"
#include "../Button.h"

namespace
{
	std::string selectSceneName = "SelectScene";
	std::string manualName = "Manual";
}

class SceneManager;
class StageObjectManager;
class SolidText;
class ImageManager;
class UIManager;
class Sky;
class ActorAttackPlayer;
class ActorCollectPlayer;

class DogSelectScene : public GameObject
{
private:
	int attackOrCollect_;	//邪魔する側か収集する側か
	int attackOrCollectInverse_;
	int walkOrFight_;
	float solidTextRotate_;
	bool padIDNum_;
	XMFLOAT3 skyPos_;
	XMFLOAT3 skyPosFly_;
	XMFLOAT3 camPos_;
	SceneManager* pSceneManager_;
	StageObjectManager* pStageObjectManager_;
	SolidText* pSolidText_;
	ImageManager* pImageManager_;
	ImageManager* pGameTitleImageManager_;
	ImageManager* pPlayerSymbolImageManager_;
	UIManager* pUIManager_;
	UIManager* pDogsSelectUIManager_;
	Button* buttonStart_;
	Button* buttonBack_;
	Sky* pSky_;
	ActorAttackPlayer* pActorAttackPlayer_;
	ActorCollectPlayer* pActorCollectPlayer_;
public:
	DogSelectScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < 0); }
};

