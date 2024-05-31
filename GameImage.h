#pragma once
//�C���N���[�h
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
	int hPict_;	//�摜�ԍ�
public:
	GameImage(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetMode(int _mode);
};

