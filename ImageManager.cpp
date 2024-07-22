#include <chrono>
#include "Engine/ImGui/imgui.h"
#include "Engine/Image.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "Player/PlayerBase.h"
#include "Player/CollectPlayer.h"
#include "Player/AttackPlayer.h"
#include "ImageManager.h"

using enum ImageManager::IMAGESTATE;
using enum ImageManager::GAUGESTATE;
using enum ImageManager::PLAYERSYMBOLSTATE;
using enum ImageManager::BONESTATE;
using enum PADIDSTATE;
using enum MOUSESTATE;
using enum PLAYSCENESTATE;

ImageManager::ImageManager(GameObject* _pParent)
	:GameObject(_pParent, gameImageName), hModel_{}, hTimeGaugePict_{}, hClickButtonPict_{}, hPlayerWinPict_{}
	, hManualPict_{}, hButtonPict_{},hBonePict_{},hYellowBonePict_{},hPlayerSymbolPict_{}
	,imageState_{GAMEOVERIMAGE},gaugeState_{WALKGAUGE}, isMatchWinner_{}
	, imageWidth_{}, imageHeight_{}, left_{}, width_{}, nowPw_{ 0.1f }, gaugeTransform_{},gaugeFrameTransform_{},playerSymbolTransformPrev_{}
	, imageTransform_{}, buttonTransform_{},boneTransform_{},playerSymbolTransform_{}
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
	for (int i = 0; i < (int)MAXBONENUM; ++i)
	{
		boneTransform_[i].position_ = {-0.9f + i * 0.15f,0.9f,0.0f};
		boneTransform_[i].scale_ = {0.2f,0.2f,0.2f};
	}
}

void ImageManager::Update()
{
	if (imageState_ == GAMETITLEIMAGE)
	{
		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)SECONDS) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)FIRST))
		{
			buttonTransform_.scale_ = { 0.5f,0.5f,0.5f };
		}
		else
		{
			buttonTransform_.scale_ = { 0.3f,0.3f,0.3f };
		}
	}
	if (imageState_ == PLAYERSYMBOLIMAGE)
	{
		if (attackOrCollect_ == (int)FIRST)
		{
			/*playerSymbolTransformPrev_ = playerSymbolTransform_[(int)ONEP];
			playerSymbolTransform_[(int)ONEP] = playerSymbolTransform_[(int)TWOP];
			playerSymbolTransform_[(int)TWOP] = playerSymbolTransformPrev_;*/
		}
	}
}

void ImageManager::BothViewDraw()
{
	if(imageState_ == GAMEOVERIMAGE)
	{
		Image::SetTransform(hPlayerWinPict_, imageTransform_);
		Image::Draw(hPlayerWinPict_);
	}
	if (imageState_ == GAMETITLEIMAGE)
	{
		Image::SetTransform(hButtonPict_, buttonTransform_);
		Image::Draw(hButtonPict_);
		Image::SetTransform(hClickButtonPict_, imageTransform_);
		Image::Draw(hClickButtonPict_);
	}
	if (imageState_ == GAMEMANUALIMAGE)
	{
		Image::SetTransform(hManualPict_, transform_);
		Image::Draw(hManualPict_);
	}
	if (imageState_ == PLAYERSYMBOLIMAGE)
	{
		for (int i = 0; i < (int)PLAYERSYMBOLNUM; i++)
		{
			Image::SetTransform(hPlayerSymbolPict_[i], playerSymbolTransform_[i]);
			Image::Draw(hPlayerSymbolPict_[i]);
		}
	}
}

void ImageManager::LeftViewDraw()
{
	if (imageState_ == BONEIMAGE)
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
	if (imageState_ == BONEIMAGE)
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
	case GAMEOVERIMAGE:
		break;
	case GAMETITLEIMAGE:
		break;
	case GAMEMANUALIMAGE:
		break;
	case TIMEGAUGEIMAGE:

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
			if (walkOrFight_ == (int)DOGSWALK)
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
			if (walkOrFight_ == (int)DOGSFIGHT)
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
			if (walkOrFight_ == (int)DOGSFIGHT)
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
	if (pCollectPlayer_->GetPadID() == (int)FIRST)
	{
		if (walkOrFight_ == (int)DOGSWALK)
		{
			BoneDraw(walkBoneNum_);
		}
		if (walkOrFight_ == (int)DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::RightCollectDraw()
{
	if (pCollectPlayer_->GetPadID() == (int)SECONDS)
	{
		if (walkOrFight_ == (int)DOGSWALK)
		{
			BoneDraw(walkBoneNum_);
		}
		if (walkOrFight_ == (int)DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::LeftAttackDraw()
{
	if (pAttackPlayer_->GetPadID() == (int)FIRST)
	{
		if (walkOrFight_ == (int)DOGSFIGHT)
		{
			BoneDraw(fightBoneNum_);
		}
	}
}

void ImageManager::RightAttackDraw()
{
	if (pAttackPlayer_->GetPadID() == (int)SECONDS)
	{
		if (walkOrFight_ == (int)DOGSFIGHT)
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
	case GAMEOVERIMAGE:
		imageTransform_.position_ = { 0.0f,0.8f,0.0f };
		if (attackPlayerScore_ < collectPlayerScore_)
		{
			if (attackOrCollectInverse_ == (int)FIRST)
			{
				isMatchWinner_ = (int)FIRST;
			}
			if (attackOrCollectInverse_ == (int)SECONDS)
			{
				isMatchWinner_ = (int)SECONDS;
			}
		}
		if (attackPlayerScore_ > collectPlayerScore_)
		{
			if (attackOrCollect_ == (int)FIRST)
			{
				isMatchWinner_ = (int)FIRST;
			}
			if (attackOrCollect_ == (int)SECONDS)
			{
				isMatchWinner_ = (int)SECONDS;
			}
		}
		//画像データのロード
		if (isMatchWinner_ == (int)FIRST)
		{
			hPlayerWinPict_ = Image::Load(modelFolderName + "Player1Win" + imageModifierName);
		}
		if (isMatchWinner_ == (int)SECONDS)
		{
			hPlayerWinPict_ = Image::Load(modelFolderName + "Player2Win" + imageModifierName);
		}
		assert(hPlayerWinPict_ >= 0);
		break;
	case GAMETITLEIMAGE:
		hClickButtonPict_ = Image::Load(modelFolderName + "ClickButton" + imageModifierName);
		assert(hClickButtonPict_ >= 0);
		hButtonPict_ = Image::Load(modelFolderName + "BButton" + imageModifierName);
		assert(hButtonPict_ >= 0);
		imageTransform_.position_ = { 0.3f,-0.5f,0.0f };
		break;
	case GAMEMANUALIMAGE:
		hManualPict_ = Image::Load(modelFolderName + "Manual" + imageModifierName);
		assert(hManualPict_ >= 0);
		break;
	case TIMEGAUGEIMAGE:
		hTimeGaugePict_ = Image::Load(modelFolderName + "TimeGauge" + imageModifierName);
		assert(hTimeGaugePict_ >= 0);
		imageWidth_ = Image::GetWidth(hTimeGaugePict_);
		imageHeight_ = Image::GetHeight(hTimeGaugePict_);
		hFramePict_ = Image::Load(modelFolderName + "TimeGaugeFlame" + imageModifierName);
		assert(hFramePict_ >= 0);
		break;
	case BONEIMAGE:
		pCollectPlayer_ = (CollectPlayer*)FindObject(collectPlayerName);
		pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
		for (int i = 0; i < (int)MAXBONENUM; ++i)
		{
			hBonePict_[i] = Image::Load(modelFolderName + "Bone" + imageModifierName);
			assert(hBonePict_[i] >= 0);
			hYellowBonePict_[i] = Image::Load(modelFolderName + "YellowBone" + imageModifierName);
			assert(hYellowBonePict_[i] >= 0);
		}
		break;
	case PLAYERSYMBOLIMAGE:

		hPlayerSymbolPict_[(int)ONEP] = Image::Load(modelFolderName + "ONEP" + imageModifierName);
		assert(hPlayerSymbolPict_[(int)ONEP] >= 0);
		hPlayerSymbolPict_[(int)TWOP] = Image::Load(modelFolderName + "TWOP" + imageModifierName);
		assert(hPlayerSymbolPict_[(int)TWOP] >= 0);
		playerSymbolTransform_[(int)ONEP].position_ = XMFLOAT3(5.0f, 1.0f, 0.0f);
		playerSymbolTransform_[(int)TWOP].position_ = XMFLOAT3(-5.0f,1.0f, 0.0f);
		break;
	}

	if (gaugeState_ == WALKGAUGE)
	{
		gaugeTransform_.position_ = { -0.96f,0.0f,0.0f };
		gaugeTransform_.scale_ = { 4.8f,9.5f,1.0f };
		gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
		gaugeFrameTransform_.scale_ = { 5.0f,12.0f,1.0f };
	}
	if (attackOrCollectInverse_ == (int)FIRST)
	{
		if (gaugeState_ == FIGHTATTACKGAUGE)
		{
			gaugeTransform_.position_ = { 0.02f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { 0.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
		if (gaugeState_ == FIGHTCOLLECTGAUGE)
		{
			gaugeTransform_.position_ = { -0.98f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
	}
	if (attackOrCollectInverse_ == (int)SECONDS)
	{
		if (gaugeState_ == FIGHTATTACKGAUGE)
		{
			gaugeTransform_.position_ = { -0.98f,0.0f,0.0f };
			gaugeTransform_.scale_ = { 2.4f,9.5f,1.0f };
			gaugeFrameTransform_.position_ = { -1.0f,0.0f,0.0f };
			gaugeFrameTransform_.scale_ = { 2.5f,12.0f,1.0f };
		}
		if (gaugeState_ == FIGHTCOLLECTGAUGE)
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