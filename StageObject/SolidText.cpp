#include "../Engine/Direct3D.h"
#include "../Engine/Global.h"
#include "SolidText.h"

SolidText::SolidText(GameObject* _pParent)
	:GameObject(_pParent, solidTextName), hModel_{ -1,-1, -1 },textState_{TEXTSTATE::SELECT}, SetRotateInitialize_{180}
{
}

void SolidText::Initialize()
{
	//▼モデルデータのロード
	std::string modelName;
	for (int i = initZeroInt; i < sizeof(solidTextModelNames) / sizeof(solidTextModelNames[initZeroInt]); i++)
	{
		modelName = modelFolderName + solidTextModelNames[i] + modelModifierName;
		hModel_[i] = Model::Load(modelName);
	}
	transform_.rotate_.y = SetRotateInitialize_;
	transform_.scale_ = { 0.6f,0.6f,0.6f };
}

void SolidText::Update()
{
}

void SolidText::Draw()
{
	switch (textState_)
	{
	case TEXTSTATE::GAMEOVER: 
		Model::SetTransform(hModel_[static_cast<int>(TEXTSTATE::GAMEOVER)], transform_);
		Model::Draw(hModel_[static_cast<int>(TEXTSTATE::GAMEOVER)]);
		break;
	case TEXTSTATE::SELECT:
		Model::SetTransform(hModel_[static_cast<int>(TEXTSTATE::SELECT)], transform_);
		Model::Draw(hModel_[static_cast<int>(TEXTSTATE::SELECT)]);
		break;
	case TEXTSTATE::GAMETITLE:
		Model::SetTransform(hModel_[static_cast<int>(TEXTSTATE::GAMETITLE)], transform_);
		Model::Draw(hModel_[static_cast<int>(TEXTSTATE::GAMETITLE)]);
	}
}

void SolidText::Release()
{
}

void SolidText::SetMode(int _mode)
{
	textState_ = static_cast<TEXTSTATE>(_mode);
}
