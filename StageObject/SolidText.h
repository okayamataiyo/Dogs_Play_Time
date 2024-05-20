#pragma once
//インクルード
#include "../Engine/Model.h"
#include "../Engine/GameObject.h"

namespace
{
	std::string solidTextModelNames[] =
	{
		"GameOverText",
		"DogsWalkText",
		"SelectText",
		"GameTitleText",
	};

	std::string solidTextName = "SolidText";
}

enum class TEXTSTATE
{
	GAMEOVER = 0,
	GAMESELECT,
	SELECT,
	GAMETITLE,
};

class SolidText : public GameObject
{
private:

	TEXTSTATE textState_;
	int hModel_[4];
public:
	SolidText(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetMode(int _mode);
};

