#include <chrono>
#include "Engine/ImGui/imgui.h"
#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "Player/PlayerBase.h"
#include "Player/CollectPlayer.h"
#include "Player/AttackPlayer.h"
#include "ImageManager.h"


ImageManager::ImageManager(GameObject* _pParent)
	:GameObject(_pParent, gameImageName), hModel_{}, hTimeGaugePict_{}, hClickButtonPict_{}, hPlayerWinPict_{}
	, hManualPict_{}, hButtonPict_{},hBonePict_{},hYellowBonePict_{}
	,imageState_{IMAGESTATE::GAMEOVER},gaugeState_{GAUGESTATE::WALK}, isMatchWinner_{}
	, imageWidth_{}, imageHeight_{}, left_{}, width_{}, nowPw_{ 0.1f }, gaugeTransform_{},gaugeFrameTransform_{}
	, imageTransform_{}, buttonTransform_{},boneTransform_{}
	, pParent_{ _pParent },pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
}

void ImageManager::Initialize()
{
	//▼INIファイルからデータのロード
	attackPlayerScore_ = GetPrivateProfileInt("PLAYERSCORE", "AttackPlayerScore", 0, "Setting/PlayerSetting.ini");
	collectPlayerScore_ = GetPrivateProfileInt("PLAYERSCORE", "CollectPlayerScore", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
	buttonTransform_.position_ = { -0.3f,-0.5f,0.0f };
	for (int i = 0; i < (int)BONESTATE::BONENUM; ++i)
	{
		boneTransform_[i].position_ = {-0.9f + i * 0.15f,0.9f,0.0f};
		boneTransform_[i].scale_ = {0.2f,0.2f,0.2f};
	}
}

void ImageManager::Update()
{
	if (imageState_ == IMAGESTATE::GAMETITLE)
	{
		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::FIRST))
		{
			buttonTransform_.scale_ = { 0.5f,0.5f,0.5f };
		}
		else
		{
			buttonTransform_.scale_ = { 0.3f,0.3f,0.3f };
		}
	}
}

void ImageManager::BothViewDraw()
{
	if(imageState_ == IMAGESTATE::GAMEOVER)
	{
		Image::SetTransform(hPlayerWinPict_, imageTransform_);
		Image::Draw(hPlayerWinPict_);
	}
	if (imageState_ == IMAGESTATE::GAMETITLE)
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
	if (imageState_ == IMAGESTATE::BONE)
	{ 
		if (pParent_->GetObjectName() == collectPlayerName)
		{
			LeftCollectDraw();
		}
		if (pParent_->GetObjectName() == attackPlayerName)
		{
			LeftAttackDraw();
		}
	}
}

void ImageManager::RightViewDraw()
{
	if (imageState_ == IMAGESTATE::BONE)
	{
		if (pParent_->GetObjectName() == collectPlayerName)
		{
			RightCollectDraw();
		}
		if (pParent_->GetObjectName() == attackPlayerName)
		{
			RightAttackDraw();
		}
	}
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

void ImageManager::BoneDraw(int _boneNum)
{
	for (int i = 0; i < _boneNum; ++i)
	{
		if (pParent_->GetObjectName() == collectPlayerName)
		{
			if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
			{
				if (pCollectPlayer_->GetScore() >= (i + 1) * 10)
				{
					Image::SetTransform(hYellowBonePict_[i], boneTransform_[i]);
					Image::Draw(hYellowBonePict_[i]);
				}
				else
				{
					Image::SetTransform(hBonePict_[i], boneTransform_[i]);
					Image::Draw(hBonePict_[i]);
				}
			}
			if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
			{
				if (pCollectPlayer_->GetIsBoneTatch())
				{
					Image::SetTransform(hYellowBonePict_[i], boneTransform_[i]);
					Image::Draw(hYellowBonePict_[i]);
				}
				else
				{
					Image::SetTransform(hBonePict_[i], boneTransform_[i]);
					Image::Draw(hBonePict_[i]);
				}
			}
		}
		if (pParent_->GetObjectName() == attackPlayerName)
		{
			if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
			{
				if (pAttackPlayer_->GetIsBoneTatch())
				{
					Image::SetTransform(hYellowBonePict_[i], boneTransform_[i]);
					Image::Draw(hYellowBonePict_[i]);
				}
				else
				{
					Image::SetTransform(hBonePict_[i], boneTransform_[i]);
					Image::Draw(hBonePict_[i]);
				}
			}
		}
	}
}

void ImageManager::LeftCollectDraw()
{
	if (pCollectPlayer_->GetPadID() == (int)PADIDSTATE::FIRST)
	{
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
		{
			BoneDraw(walkBoneNum_);
		}
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::RightCollectDraw()
{
	if (pCollectPlayer_->GetPadID() == (int)PADIDSTATE::SECONDS)
	{
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
		{
			BoneDraw(walkBoneNum_);
		}
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::LeftAttackDraw()
{
	if (pAttackPlayer_->GetPadID() == (int)PADIDSTATE::FIRST)
	{
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::RightAttackDraw()
{
	if (pAttackPlayer_->GetPadID() == (int)PADIDSTATE::SECONDS)
	{
		if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::AddGaugeScale(float _animGauge)
{
	gaugeTransform_.scale_.x -= _animGauge / maxGauge_;
}

void ImageManager::SecInit()
{
	switch (imageState_)
	{
	case IMAGESTATE::GAMEOVER:
		imageTransform_.position_ = { 0.0f,0.8f,0.0f };
		if (attackPlayerScore_ < collectPlayerScore_)
		{
			if (attackOrCollectInverse_ == (int)PADIDSTATE::FIRST)
			{
				isMatchWinner_ = (int)PADIDSTATE::FIRST;
			}
			if (attackOrCollectInverse_ == (int)PADIDSTATE::SECONDS)
			{
				isMatchWinner_ = (int)PADIDSTATE::SECONDS;
			}
		}
		if (attackPlayerScore_ > collectPlayerScore_)
		{
			if (attackOrCollect_ == (int)PADIDSTATE::FIRST)
			{
				isMatchWinner_ = (int)PADIDSTATE::FIRST;
			}
			if (attackOrCollect_ == (int)PADIDSTATE::SECONDS)
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
	case IMAGESTATE::BONE:
		pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
		pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
		for (int i = 0; i < (int)BONESTATE::BONENUM; ++i)
		{
			hBonePict_[i] = Image::Load(modelFolderName + "Bone" + imageModifierName);
			assert(hBonePict_[i] >= 0);
			hYellowBonePict_[i] = Image::Load(modelFolderName + "YellowBone" + imageModifierName);
			assert(hYellowBonePict_[i] >= 0);
		}
		break;
	}

	if (gaugeState_ == GAUGESTATE::WALK)
	{
		gaugeTransform_.position_ = { -0.96f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 4.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 5.0f,12.0f,1.0f };
	}
	if (attackOrCollectInverse_ == (int)PADIDSTATE::FIRST)
	{
		if (gaugeState_ == GAUGESTATE::FIGHTATTACK)
		{
			gaugeTransform_.position_ = { 0.02f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { 0.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
		if (gaugeState_ == GAUGESTATE::FIGHTCOLLECT)
		{
			gaugeTransform_.position_ = { -0.98f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
	}
	if (attackOrCollectInverse_ == (int)PADIDSTATE::SECONDS)
	{
		if (gaugeState_ == GAUGESTATE::FIGHTATTACK)
		{
			gaugeTransform_.position_ = { -0.98f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
		if (gaugeState_ == GAUGESTATE::FIGHTCOLLECT)
		{
			gaugeTransform_.position_ = { 0.02f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { 0.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
	}
}

void ImageManager::SetGaugeMode(int _mode)
{
	gaugeState_ = (GAUGESTATE)_mode;
}
