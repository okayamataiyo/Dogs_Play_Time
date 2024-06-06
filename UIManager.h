#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

class Text;

namespace
{
	std::string UIName = "UI";
}

enum class UISTATE
{
	GAMEOVER = 0,
	GAMETITLE,
	GAMEMANUAL,
	DOGSSELECT,
};

class UIManager : public GameObject
{
private:
	int attackOrCollectInverse_;
	int attackPlayerScore_;
	int collectPlayerScore_;
	int hModel_[4];
	UISTATE UIState_;
	Text* pText_;
public:
	UIManager(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void Release() override;
	void SetMode(int _mode);
};

