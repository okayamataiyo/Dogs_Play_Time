#include "Engine/Text.h"
#include "UI.h"
#include "Player/AttackPlayer.h"


UI::UI(GameObject* _pParent)
	:hModel_{}
	,pText_{nullptr}
{
}

void UI::Initialize()
{
	pText_ = new Text();
	pText_->Initialize();
}

void UI::Update()
{
}

void UI::Draw()
{
	int drawScoreTextX[(int)PLAYERSTATE::playerNum] = { 500,500 };
	int drawScoreTextY[(int)PLAYERSTATE::playerNum] = { 30,60 };
	int drawScoreNumberX[(int)PLAYERSTATE::playerNum] = { 830,830 };
	int drawScoreNumberY[(int)PLAYERSTATE::playerNum] = { 30,60 };
	//▼INIファイルからデータのロード
	const int attackPlayerScore = GetPrivateProfileInt("PLAYERSCORE", "AttackPlayerScore", 0, "Setting/PlayerSetting.ini");
	const int collectPlayerScore = GetPrivateProfileInt("PLAYERSCORE", "CollectPlayerScore", 0, "Setting/PlayerSetting.ini");
	pText_->Draw(drawScoreTextX[(int)PLAYERSTATE::attackPlayer], drawScoreTextY[(int)PLAYERSTATE::attackPlayer], "AttackPlayer :Score=", true, true);
	pText_->Draw(drawScoreNumberX[(int)PLAYERSTATE::attackPlayer], drawScoreNumberY[(int)PLAYERSTATE::attackPlayer], attackPlayerScore, true, true);
	pText_->Draw(drawScoreTextX[(int)PLAYERSTATE::collectPlayer], drawScoreTextY[(int)PLAYERSTATE::collectPlayer], "CollectPlayer:Score=", true, true);
	pText_->Draw(drawScoreNumberX[(int)PLAYERSTATE::collectPlayer], drawScoreNumberY[(int)PLAYERSTATE::collectPlayer], collectPlayerScore, true, true);
}

void UI::Release()
{
}

void UI::SetMode(int _mode)
{
}
