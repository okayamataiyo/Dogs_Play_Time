#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "ImageManager.h"


ImageManager::ImageManager(GameObject* _pParent)
	:GameObject(_pParent,gameImageName),hModel_{}, hPict_{},hManualPict_{}, hButtonPict_{}, imageTransform_{}, buttonTransform_{}, imageState_{IMAGESTATE::GAMEOVER}, isMatchWinner_{}
	,imageWidth_{},imageHeight_{},left{ imageWidth_ / 2 - imageWidth_ / 2 },width{ imageWidth_}
{
}

void ImageManager::Initialize()
{
	buttonTransform_.position_ = { -0.3f,-0.5f,0.0f };
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
		hButtonPict_ = Image::Load(modelFolderName + "BButton" + imageModifierName);
		assert(hButtonPict_ >= 0);
		imageTransform_.position_ = { 0.3f,-0.5f,0.0f };
		break;
	case IMAGESTATE::GAMEMANUAL:
		hManualPict_ = Image::Load(modelFolderName + "Manual" + imageModifierName);
		assert(hManualPict_ >= 0);
		break;
	case IMAGESTATE::TIMEGAUGE:
		hPict_ = Image::Load(modelFolderName + "TimeGauge" + imageModifierName);
		assert(hPict_ >= 0);
		imageWidth_ = Image::GetWidth(hPict_);
		imageHeight_ = Image::GetHeight(hPict_);
		hFramePict_ = Image::Load(modelFolderName + "TimeGaugeFrame" + imageModifierName);
		assert(hFramePict_ >= 0);
		break;
	}
}

void ImageManager::Update()
{
	switch (imageState_)
	{
	case IMAGESTATE::GAMEOVER:
		break;
	case IMAGESTATE::GAMETITLE:
		break;
	case IMAGESTATE::GAMEMANUAL:
		break;
	case IMAGESTATE::TIMEGAUGE:
		break;
	}

}

void ImageManager::BothViewDraw()
{
	Image::SetTransform(hPict_,imageTransform_);
	Image::Draw(hPict_);
	if (imageState_ == IMAGESTATE::GAMETITLE || imageState_ == IMAGESTATE::DOGSSELECT)
	{
		Image::SetTransform(hButtonPict_, buttonTransform_);
		Image::Draw(hButtonPict_);
	}
	if (imageState_ == IMAGESTATE::GAMEMANUAL)
	{
		Image::SetTransform(hManualPict_, transform_);
		Image::Draw(hManualPict_);
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
	switch (imageState_)
	{
	case IMAGESTATE::GAMEOVER:
		break;
	case IMAGESTATE::GAMETITLE:
		break;
	case IMAGESTATE::GAMEMANUAL:
		break;
	case IMAGESTATE::TIMEGAUGE:
		Image::SetTransform(hFramePict_, imageTransform_);
		Image::Draw(hFramePict_);
		Image::SetRect(hPict_, left, 0, width, imageHeight_);
		Image::SetTransform(hFramePict_, transform_);
		Image::Draw(hFramePict_);
		break;
	}
}

void ImageManager::Release()
{
}

void ImageManager::SetMode(int _mode)
{
	imageState_ = (IMAGESTATE)_mode;
}
