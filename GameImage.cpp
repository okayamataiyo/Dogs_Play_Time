#include "Engine/Image.h"
#include "Engine/Global.h"
#include "GameImage.h"


GameImage::GameImage(GameObject* _pParent)
	:hModel_{}, hPict_{}, imageTransform_{}, imageState_{ IMAGESTATE::GAMEOVER }, isMatchWinner_{}
{
}

void GameImage::Initialize()
{
	imageTransform_.position_ = { 0.0f,0.8f,0.0f };
	
}

void GameImage::Update()
{
	//▼INIファイルからデータのロード
	const int attackPlayerScore = GetPrivateProfileInt("PLAYERSCORE", "AttackPlayerScore", 0, "Setting/PlayerSetting.ini");
	const int collectPlayerScore = GetPrivateProfileInt("PLAYERSCORE", "CollectPlayerScore", 0, "Setting/PlayerSetting.ini");
	const int attackOrCollectInverse = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	const int attackOrCollect = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	switch (imageState_)
	{
	case IMAGESTATE::GAMEOVER:
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
		break;
	}
}

void GameImage::Draw()
{
	Image::SetTransform(hPict_,imageTransform_);
	Image::Draw(hPict_);
}

void GameImage::Release()
{
}

void GameImage::SetMode(int _mode)
{
	imageState_ = (IMAGESTATE)_mode;
}
