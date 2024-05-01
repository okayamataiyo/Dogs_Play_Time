#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"
#include "../Button.h"

namespace
{
	std::string selectSceneName = "SelectScene";
	std::string manualName = "Manual";
}

class SceneManager;
class StageObjectManager;
class SolidText;
class Sky;
class ActorAttackPlayer;
class ActorCollectPlayer;

class SelectScene : public GameObject
{
private:

	enum class PADIDSTATE
	{
		FIRST = 0,
		SECONDS,
		THIRD,
		FOUR,
	};

	int hPict_;
	Transform pictureTrans_;
	float solidTextRotate_;
	bool isViewPicture_;
	bool padIDNum_;
	XMFLOAT3 skyPos_;
	XMFLOAT3 skyPosFly_;
	XMFLOAT3 camPos_;
	SceneManager* pSceneManager_;
	StageObjectManager* pStageObjectManager_;
	SolidText* pSolidText_;
	Button* buttonStart_;
	Button* buttonBack_;
	Sky* pSky_;
	ActorAttackPlayer* pActorAttackPlayer_;
	ActorCollectPlayer* pActorCollectPlayer_;
public:
	SelectScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < 0); }
};

