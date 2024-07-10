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
class AttackPlayer;

class ImageManager : public GameObject
{
private:
	const int walkBoneNum_ = 12;
	const int fightBoneNum_ = 1;
	int attackPlayerScore_;
	int collectPlayerScore_;
	int attackOrCollectInverse_;
	int attackOrCollect_;
	int walkOrFight_;
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
	Transform boneTransform_[(int)BONESTATE::BONENUM];
	IMAGESTATE imageState_;
	GAUGESTATE gaugeState_;
	int isMatchWinner_;
	GameObject* pParent_;
	CollectPlayer* pCollectPlayer_;
	AttackPlayer* pAttackPlayer_;
public:
	ImageManager(GameObject* _pParent);
	void Initialize() override;
	void SecInit();
	void Update() override;
	void BothViewDraw() override;
	void LeftViewDraw() override;
	void RightViewDraw() override;
	void UPSubViewDraw() override;
	void LeftCollectDraw();
	void RightCollectDraw();
	void LeftAttackDraw();
	void RightAttackDraw();
	void Release() override;

	void BoneDraw(int _boneNum);

	void SetMode(int _mode);

	void SetGaugeMode(int _mode);

	void AddGaugeScale(float _animGauge);

	void AddValue(float _v);

	void SetValue(float _v);

	float GetValue() { return nowPw_; }
};

