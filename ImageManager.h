#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

namespace
{
	std::string gameImageName = "GameImage";
}
enum class IMAGESTATE
{
	GAMEOVER = 0,
	GAMETITLE,
	GAMEMANUAL,
	NONE,
	TIMEGAUGE,
	BONE,
};

enum class GAUGESTATE
{
	WALK = 0,
	FIGHTATTACK,
	FIGHTCOLLECT,
};
enum class BONESTATE
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	ELEVEN,
	TWELVE,
	BONENUM,
};

class CollectPlayer;

class ImageManager : public GameObject
{
private:

	int attackPlayerScore_;
	int collectPlayerScore_;
	int attackOrCollectInverse_;
	int attackOrCollect_;
	int hModel_[4];
	int hTimeGaugePict_;	//画像番号
	int hClickButtonPict_;
	int hPlayerWinPict_;
	int hButtonPict_;
	int hManualPict_;
	int hFramePict_;
	int hBonePict_[(int)BONESTATE::BONENUM];
	int hYellowBonePict_[(int)BONESTATE::BONENUM];
	float nowPw_;
	const float maxPw_ = 5.0f;
	const float minPw_ = 0.0f;
	float animPw_;
	int imageWidth_;
	int imageHeight_;
	int left_;
	int width_;
	Transform imageTransform_;
	Transform buttonTransform_;
	Transform gaugeTransform_;
	Transform gaugeFrameTransform_;
	Transform boneTransforom_[(int)BONESTATE::BONENUM];
	IMAGESTATE imageState_;
	GAUGESTATE gaugeState_;
	int isMatchWinner_;
	CollectPlayer* pCollectPlayer_;
public:
	ImageManager(GameObject* _pParent);
	void Initialize() override;
	void SecInit();
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void Release() override;

	void BoneDraw();

	void SetMode(int _mode);

	void SetGaugeMode(int _mode);

	void AddGaugeScale(float _v);

	void AddValue(float _v);

	void SetValue(float _v);

	float GetValue() { return nowPw_; }
};

