#include "Engine/Text.h"
#include "Engine/Global.h"
#include "UI.h"
#include "Player/AttackPlayer.h"


UI::UI(GameObject* _pParent)
	:hModel_{},attackOrCollectInverse_{},attackPlayerScore_{},collectPlayerScore_{}
	,pText_{nullptr}
{
}

void UI::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	attackPlayerScore_ = GetPrivateProfileInt("PLAYERSCORE", "AttackPlayerScore", 0, "Setting/PlayerSetting.ini");
	collectPlayerScore_ = GetPrivateProfileInt("PLAYERSCORE", "CollectPlayerScore", 0, "Setting/PlayerSetting.ini");
	pText_ = new Text();
	pText_->Initialize();
}

void UI::Update()
{
}

void UI::Draw()
{
	int drawScoreTextX[(int)PLAYERSTATE::playerNum] = {};
	int drawScoreTextY[(int)PLAYERSTATE::playerNum] = {};
	int drawScoreNumberX[(int)PLAYERSTATE::playerNum] = {};
	int drawScoreNumberY[(int)PLAYERSTATE::playerNum] = {};
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
	pText_->Draw(drawScoreTextX[(int)PADIDSTATE::FIRST], drawScoreTextY[(int)PADIDSTATE::FIRST], "AttackPlayer :Score=", true, true);
	pText_->Draw(drawScoreNumberX[(int)PADIDSTATE::FIRST], drawScoreNumberY[(int)PADIDSTATE::FIRST], attackPlayerScore_, true, true);
	pText_->Draw(drawScoreTextX[(int)PADIDSTATE::SECONDS], drawScoreTextY[(int)PADIDSTATE::SECONDS], "CollectPlayer:Score=", true, true);
	pText_->Draw(drawScoreNumberX[(int)PADIDSTATE::SECONDS], drawScoreNumberY[(int)PADIDSTATE::SECONDS], collectPlayerScore_, true, true);
}

void UI::Release()
{
}

void UI::SetMode(int _mode)
{
}
