#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

namespace
{
	std::string gameImageName = "GameImage";
}
enum class IMAGESTATE
{
	GAMEOVER = 0,
	GAMETITLE,
	GAMEMANUAL,
	DOGSSELECT,
};

class ImageManager : public GameObject
{
private:

	int hModel_[4];
	int hPict_;	//画像番号
	int hbuttonhPict_;
	int hmanualhPict_;
	Transform imageTransform_;
	Transform buttonTransform_;
	IMAGESTATE imageState_;
	int isMatchWinner_;
public:
	ImageManager(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void Release() override;
	void SetMode(int _mode);
};

