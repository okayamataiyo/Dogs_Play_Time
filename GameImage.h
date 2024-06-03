#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

namespace
{

}
enum class IMAGESTATE
{
	GAMEOVER = 0,
	GAMETITLE,
};

class GameImage : public GameObject
{
private:

	int hModel_[4];
	IMAGESTATE imageState_;
	Transform imageTransform_;
	int isMatchWinner_;
	int hPict_;	//画像番号
public:
	GameImage(GameObject* _pParent,IMAGESTATE _imageState);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetMode(int _mode);
};

