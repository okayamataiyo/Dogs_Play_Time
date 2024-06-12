#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "ImageManager.h"


ImageManager::ImageManager(GameObject* _pParent)
	:GameObject(_pParent, gameImageName), hModel_{}, hTimeGaugePict_{},hClickButtonPict_{}, hPlayerWinPict_{}, hManualPict_{}, hButtonPict_{},imageState_{IMAGESTATE::GAMEOVER},gaugeState_{GAUGESTATE::WALK}, isMatchWinner_{}
	, imageWidth_{}, imageHeight_{}, left_{}, width_{}, nowPw_{ 0.1f }, gaugeTransform_{},gaugeFrameTransform_{}, imageTransform_{}, buttonTransform_{}
{
}

void ImageManager::Initialize()
{
	buttonTransform_.position_ = { -0.3f,-0.5f,0.0f };
	if (gaugeState_ == GAUGESTATE::WALK)
	{
		gaugeTransform_.position_ = { -0.96f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 4.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 5.0f,12.0f,1.0f };
	}
	if (gaugeState_ == GAUGESTATE::FIGHTATTACK)
	{
		gaugeTransform_.position_ = { -0.96f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 2.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { -0.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
	}
	if (gaugeState_ == GAUGESTATE::FIGHTCOLLECT)
	{
		gaugeTransform_.position_ = { 0.0f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 2.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { 0.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
	}
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
			hPlayerWinPict_ = Image::Load(modelFolderName + "Player1Win" + imageModifierName);
		}
		if (isMatchWinner_ == (int)PADIDSTATE::SECONDS)
		{
			hPlayerWinPict_ = Image::Load(modelFolderName + "Player2Win" + imageModifierName);
		}
		assert(hPlayerWinPict_ >= 0);
		break;
	case IMAGESTATE::GAMETITLE:
		hClickButtonPict_ = Image::Load(modelFolderName + "ClickButton" + imageModifierName);
		assert(hClickButtonPict_ >= 0);
		hButtonPict_ = Image::Load(modelFolderName + "BButton" + imageModifierName);
		assert(hButtonPict_ >= 0);
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
		hManualPict_ = Image::Load(modelFolderName + "Manual" + imageModifierName);
		assert(hManualPict_ >= 0);
		break;
	case IMAGESTATE::TIMEGAUGE:
		hTimeGaugePict_ = Image::Load(modelFolderName + "TimeGauge" + imageModifierName);
		assert(hTimeGaugePict_ >= 0);
		imageWidth_ = Image::GetWidth(hTimeGaugePict_);
		imageHeight_ = Image::GetHeight(hTimeGaugePict_);
		hFramePict_ = Image::Load(modelFolderName + "TimeGaugeFlame" + imageModifierName);
		assert(hFramePict_ >= 0);
		break;
	}
	gaugeTransform_.scale_.x -= (nowPw_ / 150.0f);
}

void ImageManager::BothViewDraw()
{
	if(imageState_ == IMAGESTATE::GAMEOVER)
	{
		Image::SetTransform(hPlayerWinPict_, imageTransform_);
		Image::Draw(hPlayerWinPict_);
	}
	if (imageState_ == IMAGESTATE::GAMETITLE || imageState_ == IMAGESTATE::DOGSSELECT)
	{
		Image::SetTransform(hButtonPict_, buttonTransform_);
		Image::Draw(hButtonPict_);
		Image::SetTransform(hClickButtonPict_, imageTransform_);
		Image::Draw(hClickButtonPict_);
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

		Image::SetTransform(hFramePict_, gaugeFrameTransform_);
		Image::Draw(hFramePict_);
		Image::SetTransform(hTimeGaugePict_, gaugeTransform_);
		Image::Draw(hTimeGaugePict_);
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

void ImageManager::AddValue(float _v)
{
	nowPw_ += _v;
	if (nowPw_ < minPw_)
	{
		nowPw_ = minPw_;
	}
	else if (nowPw_ > maxPw_)
	{
		nowPw_ = maxPw_;
	}
}

void ImageManager::SetValue(float _v)
{
	nowPw_ = _v;
	if (nowPw_ < minPw_)
	{
		nowPw_ = minPw_;
	}
	else if (nowPw_ > maxPw_)
	{
		nowPw_ = maxPw_;
	}
}

void ImageManager::SecInit()
{
	if (gaugeState_ == GAUGESTATE::WALK)
	{
		gaugeTransform_.position_ = { -0.96f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 4.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 5.0f,12.0f,1.0f };
	}
	if (gaugeState_ == GAUGESTATE::FIGHTATTACK)
	{
		gaugeTransform_.position_ = { -0.96f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 2.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { -0.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
	}
	if (gaugeState_ == GAUGESTATE::FIGHTCOLLECT)
	{
		gaugeTransform_.position_ = { 0.0f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 2.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { 0.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
	}
}

void ImageManager::SetGaugeMode(int _mode)
{
	gaugeState_ = (GAUGESTATE)_mode;
}
