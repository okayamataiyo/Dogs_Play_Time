#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "ImageManager.h"


ImageManager::ImageManager(GameObject* _pParent)
	:GameObject(_pParent,gameImageName),hModel_{}, hPict_{},hmanualhPict_{}, hbuttonhPict_{}, imageTransform_{}, buttonTransform_{}, imageState_{IMAGESTATE::GAMEOVER}, isMatchWinner_{}
{
}

void ImageManager::Initialize()
{
	buttonTransform_.position_ = { -0.3f,-0.5f,0.0f };
}

void ImageManager::Update()
{
	//▼INIファイルからデータのロード
	const int attackPlayerScore = GetPrivateProfileInt("PLAYERSCORE", "AttackPlayerScore", 0, "Setting/PlayerSetting.ini");
	const int collectPlayerScore = GetPrivateProfileInt("PLAYERSCORE", "CollectPlayerScore", 0, "Setting/PlayerSetting.ini");
	const int attackOrCollectInverse = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	const int attackOrCollect = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	switch (imageState_)
	{
	case IMAGESTATE::GAMEOVER:
		imageTransform_.position_ = { 0.0f,0.8f,0.0f };
		if (attackPlayerScore < collectPlayerScore)
		{
			if (attackOrCollectInverse == (int)PADIDSTATE::FIRST)
			{
				isMatchWinner_ = (int)PADIDSTATE::FIRST;
			}
			if (attackOrCollectInverse == (int)PADIDSTATE::SECONDS)
			{
				isMatchWinner_ = (int)PADIDSTATE::SECONDS;
			}
		}
		if (attackPlayerScore > collectPlayerScore)
		{
			if (attackOrCollect == (int)PADIDSTATE::FIRST)
			{
				isMatchWinner_ = (int)PADIDSTATE::FIRST;
			}
			if (attackOrCollect == (int)PADIDSTATE::SECONDS)
			{
				isMatchWinner_ = (int)PADIDSTATE::SECONDS;
			}
		}
		//画像データのロード
		if (isMatchWinner_ == (int)PADIDSTATE::FIRST)
		{
			hPict_ = Image::Load(modelFolderName + "Player1Win" + imageModifierName);
		}
		if (isMatchWinner_ == (int)PADIDSTATE::SECONDS)
		{
			hPict_ = Image::Load(modelFolderName + "Player2Win" + imageModifierName);
		}
		assert(hPict_ >= 0);
		break;
	case IMAGESTATE::GAMETITLE:
		hPict_ = Image::Load(modelFolderName + "ClickButton" + imageModifierName);
		assert(hPict_ >= 0);
		hbuttonhPict_ = Image::Load(modelFolderName + "BButton" + imageModifierName);
		assert(hbuttonhPict_ >= 0);
		imageTransform_.position_ = { 0.3f,-0.5f,0.0f };

		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::FIRST))
		{
			buttonTransform_.scale_ = { 0.5f,0.5f,0.5f };
		}
		else
		{
			buttonTransform_.scale_ = { 0.3f,0.3f,0.3f };
		}
		break;
	case IMAGESTATE::GAMEMANUAL:
		hmanualhPict_ = Image::Load(modelFolderName + "Manual" + imageModifierName);
		assert(hmanualhPict_ >= 0);
	}
}

void ImageManager::BothViewDraw()
{
	Image::SetTransform(hPict_,imageTransform_);
	Image::Draw(hPict_);
	if (imageState_ == IMAGESTATE::GAMETITLE || imageState_ == IMAGESTATE::DOGSSELECT)
	{
		Image::SetTransform(hbuttonhPict_, buttonTransform_);
		Image::Draw(hbuttonhPict_);
	}
	if (imageState_ == IMAGESTATE::GAMEMANUAL)
	{
		Image::SetTransform(hmanualhPict_, transform_);
		Image::Draw(hmanualhPict_);
	}
}

void ImageManager::LeftViewDraw()
{
}

void ImageManager::RightViewDraw()
{
}

void ImageManager::UPSubViewDraw()
{
}

void ImageManager::Release()
{
}

void ImageManager::SetMode(int _mode)
{
	imageState_ = (IMAGESTATE)_mode;
}
