#include "Engine/Text.h"
#include "Engine/Global.h"
#include "UIManager.h"
#include "Player/AttackPlayer.h"


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
	int drawScoreTextX[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreTextY[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreNumberX[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreNumberY[(int)PLAYERSTATE::PLAYERNUM] = {};
	switch (UIState_)
	{
	case UISTATE::GAMEOVER:
		if (attackOrCollectInverse_ == (int)PADIDSTATE::FIRST)
		{
			drawScoreTextX[(int)PADIDSTATE::FIRST] = { 900 };
			drawScoreTextY[(int)PADIDSTATE::FIRST] = { 30 };
			drawScoreTextX[(int)PADIDSTATE::SECONDS] = { 50 };
			drawScoreTextY[(int)PADIDSTATE::SECONDS] = { 30 };
			drawScoreNumberX[(int)PADIDSTATE::FIRST] = { 1230 };
			drawScoreNumberY[(int)PADIDSTATE::FIRST] = { 30 };
			drawScoreNumberX[(int)PADIDSTATE::SECONDS] = { 380 };
			drawScoreNumberY[(int)PADIDSTATE::SECONDS] = { 30 };
		}
		if (attackOrCollectInverse_ == (int)PADIDSTATE::SECONDS)
		{
			drawScoreTextX[(int)PADIDSTATE::FIRST] = { 50 };
			drawScoreTextY[(int)PADIDSTATE::FIRST] = { 30 };
			drawScoreTextX[(int)PADIDSTATE::SECONDS] = { 900 };
			drawScoreTextY[(int)PADIDSTATE::SECONDS] = { 30 };
			drawScoreNumberX[(int)PADIDSTATE::FIRST] = { 380 };
			drawScoreNumberY[(int)PADIDSTATE::FIRST] = { 30 };
			drawScoreNumberX[(int)PADIDSTATE::SECONDS] = { 1230 };
			drawScoreNumberY[(int)PADIDSTATE::SECONDS] = { 30 };
		}
		pText_->BothViewDraw(drawScoreTextX[(int)PADIDSTATE::FIRST], drawScoreTextY[(int)PADIDSTATE::FIRST], "AttackPlayer :Score=");
		pText_->BothViewDraw(drawScoreNumberX[(int)PADIDSTATE::FIRST], drawScoreNumberY[(int)PADIDSTATE::FIRST], attackPlayerScore_);
		pText_->BothViewDraw(drawScoreTextX[(int)PADIDSTATE::SECONDS], drawScoreTextY[(int)PADIDSTATE::SECONDS], "CollectPlayer:Score=");
		pText_->BothViewDraw(drawScoreNumberX[(int)PADIDSTATE::SECONDS], drawScoreNumberY[(int)PADIDSTATE::SECONDS], collectPlayerScore_);
		break;
	case UISTATE::GAMETITLE:

		break;
	case UISTATE::GAMEMANUAL:
		drawScoreTextX[(int)PADIDSTATE::FIRST] = { 50 };
		drawScoreTextY[(int)PADIDSTATE::FIRST] = { 30 };
		pText_->BothViewDraw(drawScoreTextX[(int)PADIDSTATE::FIRST], drawScoreTextY[(int)PADIDSTATE::FIRST], "XButton:MANUAL");
		break;
	case UISTATE::DOGSSELECT:
		drawScoreTextX[(int)PADIDSTATE::FIRST] = { 50 };
		drawScoreTextY[(int)PADIDSTATE::FIRST] = { 60 };
		pText_->BothViewDraw(drawScoreTextX[(int)PADIDSTATE::FIRST], drawScoreTextY[(int)PADIDSTATE::FIRST], "RStick:<- ->");
		break;
	case UISTATE::DOGSWALKATTACK:

		break;
	case UISTATE::DOGSWALKCOLLECT:
		
		break;
	}
}

void UIManager::LeftViewDraw()
{
	int drawScoreTextX[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreTextY[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreNumberX[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreNumberY[(int)PLAYERSTATE::PLAYERNUM] = {};
	switch (UIState_)
	{
		case::UISTATE::DOGSWALKATTACK:
			drawScoreTextX[(int)PADIDSTATE::FIRST] = { 30 };
			drawScoreTextY[(int)PADIDSTATE::FIRST] = { 30 };
			drawScoreNumberX[(int)PADIDSTATE::FIRST] = { 360 };
			drawScoreNumberY[(int)PADIDSTATE::FIRST] = { 30 };
			if (padNum_ == (int)PADIDSTATE::FIRST)
			{
				pText_->LeftViewDraw(drawScoreTextX[(int)PADIDSTATE::FIRST], drawScoreTextY[(int)PADIDSTATE::FIRST], "AttackPlayer:Score=");
				pText_->LeftViewDraw(drawScoreNumberX[(int)PADIDSTATE::FIRST], drawScoreNumberY[(int)PADIDSTATE::FIRST], attackPlayerScore_);
			}
			break;
		case::UISTATE::DOGSWALKCOLLECT:
			drawScoreTextX[(int)PADIDSTATE::SECONDS] = { 30 };
			drawScoreTextY[(int)PADIDSTATE::SECONDS] = { 30 };
			drawScoreNumberX[(int)PADIDSTATE::SECONDS] = { 360 };
			drawScoreNumberY[(int)PADIDSTATE::SECONDS] = { 30 };
			if (padNum_ == (int)PADIDSTATE::FIRST)
			{
				pText_->LeftViewDraw(drawScoreTextX[(int)PADIDSTATE::SECONDS], drawScoreTextY[(int)PADIDSTATE::SECONDS], "CollectPlayer:Score=");
				pText_->LeftViewDraw(drawScoreNumberX[(int)PADIDSTATE::SECONDS], drawScoreNumberY[(int)PADIDSTATE::SECONDS], collectPlayerScore_);
			}
			break;
	}
}

void UIManager::RightViewDraw()
{
	int drawScoreTextX[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreTextY[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreNumberX[(int)PLAYERSTATE::PLAYERNUM] = {};
	int drawScoreNumberY[(int)PLAYERSTATE::PLAYERNUM] = {};
	switch (UIState_)
	{
	case::UISTATE::DOGSWALKATTACK:
		drawScoreTextX[(int)PADIDSTATE::FIRST] = { 30 };
		drawScoreTextY[(int)PADIDSTATE::FIRST] = { 30 };
		drawScoreNumberX[(int)PADIDSTATE::FIRST] = { 360 };
		drawScoreNumberY[(int)PADIDSTATE::FIRST] = { 30 };
		if (padNum_ == (int)PADIDSTATE::SECONDS)
		{
			pText_->RightViewDraw(drawScoreTextX[(int)PADIDSTATE::FIRST], drawScoreTextY[(int)PADIDSTATE::FIRST], "AttackPlayer:Score=");
			pText_->RightViewDraw(drawScoreNumberX[(int)PADIDSTATE::FIRST], drawScoreNumberY[(int)PADIDSTATE::FIRST], attackPlayerScore_);
		}
		break;
	case::UISTATE::DOGSWALKCOLLECT:
		drawScoreTextX[(int)PADIDSTATE::SECONDS] = { 30 };
		drawScoreTextY[(int)PADIDSTATE::SECONDS] = { 30 };
		drawScoreNumberX[(int)PADIDSTATE::SECONDS] = { 360 };
		drawScoreNumberY[(int)PADIDSTATE::SECONDS] = { 30 };
		if (padNum_ == (int)PADIDSTATE::SECONDS)
		{
			pText_->RightViewDraw(drawScoreTextX[(int)PADIDSTATE::SECONDS], drawScoreTextY[(int)PADIDSTATE::SECONDS], "CollectPlayer:Score=");
			pText_->RightViewDraw(drawScoreNumberX[(int)PADIDSTATE::SECONDS], drawScoreNumberY[(int)PADIDSTATE::SECONDS], collectPlayerScore_);
		}
		break;
	}
}

void UIManager::Release()
{
}

void UIManager::SetMode(int _mode)
{
	UIState_ = (UISTATE)_mode;
}