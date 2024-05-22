#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"

namespace
{
	std::string gameSelectSceneName = "GameSelectScene";
}

class SceneManager;
class SolidText;
class Sky;
class ActorAttackPlayer;
class ActorCollectPlayer;

class GameSelectScene : public GameObject
{
private:

	int hPict_;
	int attackOrCollect_;
	int attackOrCollectInverse_;
	int walkOrFight_;
	bool padIDNum_;
	XMFLOAT3 skyPos_;
	XMFLOAT3 skyPosFly_;
	XMFLOAT3 camPos_;
	SceneManager* pSceneManager_;
	StageObjectManager* pStageObjectManager_;
	SolidText* pDogsWalkText_;
	SolidText* pDogsFightText_;
	Sky* pSky_;
public:
	GameSelectScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < 0); }
};

