#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

namespace
{

}

class GameImage : public GameObject
{
private:

	int hModel_[4];
	Transform imageTransform_;
	int isMatchWinner_;
	int hPict_;	//画像番号
public:
	GameImage(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetMode(int _mode);
};

