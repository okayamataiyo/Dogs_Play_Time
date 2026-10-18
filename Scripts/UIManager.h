#pragma once
//インクルード
#include "../Engine/Model.h"
#include "../Engine/GameObject.h"
#include "../Engine/Text.h"


namespace
{
	std::string UIName = "UI";
}

class UIManager : public GameObject
{
public:
	enum class UISTATE
	{
		GAMEOVERUI = 0,
		GAMETITLEUI,
		GAMEMANUALUI,
		DOGSSELECTUI,
		DOGSWALKATTACKUI,
		DOGSWALKCOLLECTUI,
		DOGSFIGHTUI,
	};
private:
	int attackOrCollectInverse_;
	int attackPlayerScore_;
	int collectPlayerScore_;
	int padNum_;
	int hModel_[4];
	UISTATE UIState_;
	std::unique_ptr<Text> pText_;	//このUIManagerが持つ
public:
	UIManager(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void Release() override;
	void SetMode(int _mode);
	void SetAttackPlayerScore(int _score) { attackPlayerScore_ = _score; }
	void SetCollectPlayerScore(int _score) { collectPlayerScore_ = _score; }
	void SetPadNum(int _padNum) { padNum_ = _padNum; }
};

