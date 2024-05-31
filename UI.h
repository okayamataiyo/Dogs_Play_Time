#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

class Text;

namespace
{

}

class UI : public GameObject
{
private:
	int attackOrCollectInverse_;
	int attackPlayerScore_;
	int collectPlayerScore_;
	int hModel_[4];
	Text* pText_;
public:
	UI(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetMode(int _mode);
};

