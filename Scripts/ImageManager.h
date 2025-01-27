#pragma once
//インクルード
#include "../Engine/Model.h"
#include "../Engine/GameObject.h"

namespace
{
	std::string gameImageName = "GameImage";
}

class CollectPlayer;
class AttackPlayer;

class ImageManager : public GameObject
{
public:
	enum class PLAYERSYMBOLSTATE
	{
		ONEP = 0,
		TWOP,
		PLAYERSYMBOLNUM,
	};

	enum class IMAGESTATE
	{
		GAMEOVERIMAGE = 0,
		GAMETITLEIMAGE,
		GAMEMANUALIMAGE,
		NONEIMAGE,
		TIMEGAUGEIMAGE,
		BONEIMAGE,
		PLAYERSYMBOLIMAGE,
	};

	enum class GAUGESTATE
	{
		WALKGAUGE = 0,
		FIGHTATTACKGAUGE,
		FIGHTCOLLECTGAUGE,
	};
	enum class BONESTATE
	{
		ONEBONE,
		TWOBONE,
		THREEBONE,
		FOURBONE,
		FIVEBONE,
		SIXBONE,
		SEVENBONE,
		EIGHTBONE,
		NINEBONE,
		TENBONE,
		ELEVENBONE,
		TWELVEBONE,
		MAXBONENUM,
	};

private:
	const int walkBoneNum_ = 12;
	const int fightBoneNum_ = 1;
	int attackPlayerScore_;
	int collectPlayerScore_;
	int attackOrCollectInverse_;
	int attackOrCollect_;
	int attackOrCollectPrev_;
	int walkOrFight_;
	int hModel_[4];
	int hTimeGaugePict_;	//画像番号
	int hClickButtonPict_;
	int hPlayerWinPict_;
	int hButtonPict_;
	int hManualPict_;
	int hFramePict_;
	int hBonePict_[(int)BONESTATE::MAXBONENUM];
	int hYellowBonePict_[(int)BONESTATE::MAXBONENUM];
	int hPlayerSymbolPict_[(int)PLAYERSYMBOLSTATE::PLAYERSYMBOLNUM];
	float nowPw_;
	const float maxPw_ = 5.0f;
	const float minPw_ = 0.0f;
	float animPw_;
	int imageWidth_;
	int imageHeight_;
	int left_;
	int width_;
	const float maxGauge_ = 120;
	Transform imageTransform_;
	Transform buttonTransform_;
	Transform gaugeTransform_;
	Transform gaugeFrameTransform_;
	Transform boneTransform_[(int)BONESTATE::MAXBONENUM];
	Transform playerSymbolTransform_[(int)PLAYERSYMBOLSTATE::PLAYERSYMBOLNUM];
	Transform playerSymbolTransformPrev_;
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

	void SetAttackOrCollect(int _attackOrCollect) { attackOrCollect_ = _attackOrCollect; }

	void SetAttackOrCollectInverse(int _attackOrCollectInverse) { attackOrCollectInverse_ = _attackOrCollectInverse; }

	void AddGaugeScale(float _animGauge);

	void AddValue(float _v);

	void SetValue(float _v);

	float GetValue() { return nowPw_; }
};

