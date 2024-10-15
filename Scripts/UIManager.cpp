#include "../Engine/Text.h"
#include "../Engine/Global.h"
#include "UIManager.h"
#include "Player/AttackPlayer.h"

using enum UISTATE;
using enum PADIDSTATE;
using enum PLAYERSTATE;


UIManager::UIManager(GameObject* _pParent)
	:GameObject(_pParent,UIName),hModel_{},attackOrCollectInverse_{},attackPlayerScore_{},collectPlayerScore_{}
	,pText_{nullptr}
{
}

void UIManager::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	attackPlayerScore_ = GetPrivateProfileInt("PLAYERSCORE", "AttackPlayerScore", 0, "Setting/PlayerSetting.ini");
	collectPlayerScore_ = GetPrivateProfileInt("PLAYERSCORE", "CollectPlayerScore", 0, "Setting/PlayerSetting.ini");
	pText_ = new Text();
	pText_->Initialize();
}

void UIManager::Update()
{

}

void UIManager::BothViewDraw()
{
	int drawScoreTextX[(int)PLAYERNUM] = {};
	int drawScoreTextY[(int)PLAYERNUM] = {};
	int drawScoreNumberX[(int)PLAYERNUM] = {};
	int drawScoreNumberY[(int)PLAYERNUM] = {};
	switch (UIState_)
	{
	case UISTATE::GAMEOVERUI:
		if (attackOrCollectInverse_ == (int)FIRST)
		{
			drawScoreTextX[(int)FIRST] = { 900 };
			drawScoreTextY[(int)FIRST] = { 30 };
			drawScoreTextX[(int)SECONDS] = { 50 };
			drawScoreTextY[(int)SECONDS] = { 30 };
			drawScoreNumberX[(int)FIRST] = { 1230 };
			drawScoreNumberY[(int)FIRST] = { 30 };
			drawScoreNumberX[(int)SECONDS] = { 380 };
			drawScoreNumberY[(int)SECONDS] = { 30 };
		}
		if (attackOrCollectInverse_ == (int)SECONDS)
		{
			drawScoreTextX[(int)FIRST] = { 50 };
			drawScoreTextY[(int)FIRST] = { 30 };
			drawScoreTextX[(int)SECONDS] = { 900 };
			drawScoreTextY[(int)SECONDS] = { 30 };
			drawScoreNumberX[(int)FIRST] = { 380 };
			drawScoreNumberY[(int)FIRST] = { 30 };
			drawScoreNumberX[(int)SECONDS] = { 1230 };
			drawScoreNumberY[(int)SECONDS] = { 30 };
		}
		pText_->BothViewDraw(drawScoreTextX[(int)FIRST], drawScoreTextY[(int)FIRST], "AttackPlayer :Score=");
		pText_->BothViewDraw(drawScoreNumberX[(int)FIRST], drawScoreNumberY[(int)FIRST], attackPlayerScore_);
		pText_->BothViewDraw(drawScoreTextX[(int)SECONDS], drawScoreTextY[(int)SECONDS], "CollectPlayer:Score=");
		pText_->BothViewDraw(drawScoreNumberX[(int)SECONDS], drawScoreNumberY[(int)SECONDS], collectPlayerScore_);
		break;
	case UISTATE::GAMETITLEUI:

		break;
	case UISTATE::GAMEMANUALUI:
		drawScoreTextX[(int)FIRST] = { 50 };
		drawScoreTextY[(int)FIRST] = { 30 };
		pText_->BothViewDraw(drawScoreTextX[(int)FIRST], drawScoreTextY[(int)FIRST], "XButton:MANUAL");
		break;
	case UISTATE::DOGSSELECTUI:
		drawScoreTextX[(int)FIRST] = { 50 };
		drawScoreTextY[(int)FIRST] = { 60 };
		pText_->BothViewDraw(drawScoreTextX[(int)FIRST], drawScoreTextY[(int)FIRST], "RStick:<- ->");
		break;
	case UISTATE::DOGSWALKATTACKUI:

		break;
	case UISTATE::DOGSWALKCOLLECTUI:
		
		break;
	}
}

void UIManager::LeftViewDraw()
{
	int drawScoreTextX[(int)PLAYERNUM] = {};
	int drawScoreTextY[(int)PLAYERNUM] = {};
	int drawScoreNumberX[(int)PLAYERNUM] = {};
	int drawScoreNumberY[(int)PLAYERNUM] = {};
	switch (UIState_)
	{
		case::UISTATE::DOGSWALKATTACKUI:
			drawScoreTextX[(int)FIRST] = { 30 };
			drawScoreTextY[(int)FIRST] = { 30 };
			drawScoreNumberX[(int)FIRST] = { 360 };
			drawScoreNumberY[(int)FIRST] = { 30 };
			if (padNum_ == (int)FIRST)
			{
				pText_->LeftViewDraw(drawScoreTextX[(int)FIRST], drawScoreTextY[(int)FIRST], "AttackPlayer:Score=");
				pText_->LeftViewDraw(drawScoreNumberX[(int)FIRST], drawScoreNumberY[(int)FIRST], attackPlayerScore_);
			}
			break;
		case::UISTATE::DOGSWALKCOLLECTUI:
			drawScoreTextX[(int)SECONDS] = { 30 };
			drawScoreTextY[(int)SECONDS] = { 30 };
			drawScoreNumberX[(int)SECONDS] = { 360 };
			drawScoreNumberY[(int)SECONDS] = { 30 };
			if (padNum_ == (int)FIRST)
			{
				pText_->LeftViewDraw(drawScoreTextX[(int)SECONDS], drawScoreTextY[(int)SECONDS], "CollectPlayer:Score=");
				pText_->LeftViewDraw(drawScoreNumberX[(int)SECONDS], drawScoreNumberY[(int)SECONDS], collectPlayerScore_);
			}
			break;
	}
}

void UIManager::RightViewDraw()
{
	int drawScoreTextX[(int)PLAYERNUM] = {};
	int drawScoreTextY[(int)PLAYERNUM] = {};
	int drawScoreNumberX[(int)PLAYERNUM] = {};
	int drawScoreNumberY[(int)PLAYERNUM] = {};
	switch (UIState_)
	{
	case::UISTATE::DOGSWALKATTACKUI:
		drawScoreTextX[(int)FIRST] = { 30 };
		drawScoreTextY[(int)FIRST] = { 30 };
		drawScoreNumberX[(int)FIRST] = { 360 };
		drawScoreNumberY[(int)FIRST] = { 30 };
		if (padNum_ == (int)SECONDS)
		{
			pText_->RightViewDraw(drawScoreTextX[(int)FIRST], drawScoreTextY[(int)FIRST], "AttackPlayer:Score=");
			pText_->RightViewDraw(drawScoreNumberX[(int)FIRST], drawScoreNumberY[(int)FIRST], attackPlayerScore_);
		}
		break;
	case::UISTATE::DOGSWALKCOLLECTUI:
		drawScoreTextX[(int)SECONDS] = { 30 };
		drawScoreTextY[(int)SECONDS] = { 30 };
		drawScoreNumberX[(int)SECONDS] = { 360 };
		drawScoreNumberY[(int)SECONDS] = { 30 };
		if (padNum_ == (int)SECONDS)
		{
			pText_->RightViewDraw(drawScoreTextX[(int)SECONDS], drawScoreTextY[(int)SECONDS], "CollectPlayer:Score=");
			pText_->RightViewDraw(drawScoreNumberX[(int)SECONDS], drawScoreNumberY[(int)SECONDS], collectPlayerScore_);
		}
		break;
	}
}

void UIManager::UPSubViewDraw()
{
}

void UIManager::Release()
{
}

void UIManager::SetMode(int _mode)
{
	UIState_ = (UISTATE)_mode;
}