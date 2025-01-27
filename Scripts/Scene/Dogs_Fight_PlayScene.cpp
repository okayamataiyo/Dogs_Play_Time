//インクルード
#include <random>
#include "../../Engine/Input.h"
#include "../../Engine/SceneManager.h"
#include "../../Engine/Audio.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "../Player/AIPlayer.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/FrameBox.h"
#include "../ItemObject/BoneSuck.h"
#include "../ItemObject/Bone.h"
#include "../StageObject/StageObjectManager.h"
#include "../ImageManager.h"
#include "Dogs_Fight_PlayScene.h"

using enum ImageManager::IMAGESTATE;
using enum STAGEOBJECTSTATE;
using enum ITEMOBJECTSTATE;
using enum PADIDSTATE;
using enum PLAYERNUMSTATE;
using enum ImageManager::GAUGESTATE;
using enum Dogs_Fight_PlayScene::SOUNDSTATE;

Dogs_Fight_PlayScene::Dogs_Fight_PlayScene(GameObject* _pParent)
	:GameObject(_pParent,Dogs_Fight_PlaySceneName),attackOrCollect_{0},attackOrCollectInverse_{0}
	,stageBlockNum_{3},lengthRecedes_{5},degreesMin_{0.0f},degreesMax_{-88.0f},degreesToRadians_{3.14f / 180.0f},vecLengthRecedes_{1.0f},vecLengthApproach_{1.0f}
	,woodBoxCountMax_{5},changeScore_{100},hSound_{-1,-1,-1,-1}
	, random_value_{ 0 },soundVolume_{0.05f},soundVolumeHalf_{soundVolume_ / 2.0f},length_{30.0f},boneCount_{0}
	, woodBoxCount_{ 0 }, attackPlayerPosition_{}, attackPlayerDirection_{}, woodBoxFrontPosition_{ 10.0f }, isGameStop_{ false }
	,isPause_{nullptr}
	, pSceneManager_{ nullptr }, pAttackPlayer_{ nullptr }, pCollectPlayer_{ nullptr },pAIPlayer_{nullptr}
	, pItemObjectManager_{ nullptr }, pStageObjectManager_{ nullptr },pAttackImageManager_{nullptr},pCollectImageManager_{nullptr}
{
}

void Dogs_Fight_PlayScene::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	//▼サウンドデータのロード
	std::string soundName;
	for (int i = 0u; i < sizeof(soundDogs_Fight_PlaySceneNames) / sizeof(soundDogs_Fight_PlaySceneNames[initZeroInt]); i++)
	{
		soundName = soundFolderName + soundDogs_Fight_PlaySceneNames[i] + soundModifierName;
		hSound_[i] = Audio::Load(soundName);
		assert(hSound_[i] >= 0u);
	}
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pItemObjectManager_ = new ItemObjectManager(this);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(SKY);
	pStageObjectManager_->CreateStageObjectOrigin(STAGE);

	float stageBlockSummonsPosLimitMinX = 100.0f;
	float stageBlockSummonsPosLimitMaxX = 100.0f;
	float stageBlockSummonsPosLimitMinZ = 100.0f;
	float stageBlockSummonsPosLimitMaxZ = 100.0f;

	for (int i = 0u; i < stageBlockNum_; i++)
	{
		pStageObjectManager_->CreateStageObject(STAGEBLOCK, -stageBlockSummonsPosLimitMinX, stageBlockSummonsPosLimitMaxX, -stageBlockSummonsPosLimitMinZ, stageBlockSummonsPosLimitMaxZ);
	}
	floorPosition_[0].position_ = { 30.0f,0.8f,3.0f };
	floorPosition_[1].position_ = { -10.0f,0.5f,50.0f };
	floorPosition_[2].position_ = { -45.0f, 0.3f,-43.0f };
	XMFLOAT3 scale = { 3.0f,1.0f,3.0f };
	XMFLOAT3 DefaultData[2] = { XMFLOAT3(0.0f,0.0f,0.0f)	//0で初期化
							   ,XMFLOAT3(1.0f,1.0f,1.0f) };	//1で初期化
	XMFLOAT3 WoodBox[3] = { XMFLOAT3(30.0f,-5.0f,40.0f)
							  ,XMFLOAT3(20.0f,-10.0f,40.0f)
							  ,XMFLOAT3(10.0f,-20.0f,40.0f) };
	XMFLOAT3 FrameBox = { XMFLOAT3(5.0f,5.0f,5.0f) };

	pItemObjectManager_->CreateObject(FLOOR, floorPosition_[2].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));
	pItemObjectManager_->CreateObject(FLOOR, floorPosition_[1].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));

	pAttackPlayer_ = Instantiate<AttackPlayer>(this);
	camVec_[attackOrCollect_] = XMFLOAT3(0, 5, -10);
	pCollectPlayer_ = Instantiate<CollectPlayer>(this);
	camVec_[attackOrCollectInverse_] = XMFLOAT3(0, 5, -10);
	pAIPlayer_ = Instantiate<AIPlayer>(this);
	pItemObjectManager_->CreateObject(FRAMEBOX, DefaultData[0], DefaultData[1], FrameBox);
	pAttackPlayer_->SetCollectPlayer(pCollectPlayer_);
	pAttackPlayer_->SetAIPlayer(pAIPlayer_);
	pCollectPlayer_->SetAttackPlayer(pAttackPlayer_);
	pCollectPlayer_->SetAIPlayer(pAIPlayer_);
	pAIPlayer_->SetCollectPlayer(pCollectPlayer_);
	pAIPlayer_->SetAttackPlayer(pAttackPlayer_);
	playerFirstPos_[(int)ATTACKPLAYER] = XMFLOAT3(0,0, 8);
	playerFirstPos_[(int)COLLECTPLAYER] = XMFLOAT3(0, 0, -8);
	playerFirstPos_[(int)AIPLAYER] = XMFLOAT3(0, 0, 0);
	pAttackPlayer_->SetPosition(playerFirstPos_[(int)ATTACKPLAYER]);
	pCollectPlayer_->SetPosition(playerFirstPos_[(int)COLLECTPLAYER]);
	pAIPlayer_->SetPosition(playerFirstPos_[(int)AIPLAYER]);
	pAttackPlayer_->SetImageSecInit();
	pCollectPlayer_->SetImageSecInit();

	//乱数生成器の設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 2);

	//1から2までのランダムな値の作成
	random_value_ = dis(gen);

	pAttackImageManager_ = Instantiate<ImageManager>(this);
	pAttackImageManager_->SetMode((int)TIMEGAUGEIMAGE);
	pAttackImageManager_->SetGaugeMode((int)FIGHTATTACKGAUGE);
	pAttackImageManager_->SecInit();
	pCollectImageManager_ = Instantiate<ImageManager>(this);
	pCollectImageManager_->SetMode((int)TIMEGAUGEIMAGE);
	pCollectImageManager_->SetGaugeMode((int)FIGHTCOLLECTGAUGE);
	pCollectImageManager_->SecInit();
}

void Dogs_Fight_PlayScene::Update()
{
	if (pAttackPlayer_->GetIsBoneTatch())
	{
		pAttackImageManager_->AddGaugeScale(0.08f);
	}
	if (pCollectPlayer_->GetIsBoneTatch())
	{
		pCollectImageManager_->AddGaugeScale(0.08f);
	}
	if ((!isGameStop_ && pAttackPlayer_->GetScore() >= changeScore_) || (!isGameStop_ && pCollectPlayer_->GetScore() >= changeScore_))
	{
		Audio::Stop(hSound_[(int)BGM]);
		Audio::Play(hSound_[random_value_], soundVolume_);
	}
	if ((!isGameStop_ && pAttackPlayer_->GetScore() < changeScore_) || (!isGameStop_ && pCollectPlayer_->GetScore() < changeScore_))
	{
		Audio::Play(hSound_[(int)BGM], soundVolumeHalf_);
	}
	//木箱が邪魔側の犬の前にくるための計算
	attackPlayerPosition_ = pAttackPlayer_->GetPosition();
	attackPlayerDirection_ = XMLoadFloat3(&attackPlayerPosition_) - Camera::VecGetPosition(pAttackPlayer_->GetPadID());
	attackPlayerDirection_ = XMVectorSetY(attackPlayerDirection_, 0.0f);	//y座標の初期化
	attackPlayerDirection_ = XMVector3Normalize(attackPlayerDirection_);
	attackPlayerPosition_.x += woodBoxFrontPosition_ * XMVectorGetX(attackPlayerDirection_);
	attackPlayerPosition_.y += -2.0f;
	attackPlayerPosition_.z += woodBoxFrontPosition_ * XMVectorGetZ(attackPlayerDirection_);
	if (Input::IsKeyDown(DIK_P))
	{
		isPause_ = !isPause_;
	}
	HideCursor();
	BoneSummons();
	if (woodBoxCount_ <= woodBoxCountMax_)
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_Y, pAttackPlayer_->GetPadID()) && pAttackPlayer_->GetIsJump())
		{
			XMFLOAT3 woodBoxRotate = {};
			woodBoxRotate.y = pAttackPlayer_->GetAngle();
			pItemObjectManager_->CreateObject(WOODBOX, attackPlayerPosition_, woodBoxRotate, XMFLOAT3(0.5f, 0.5f, 0.5f));
			woodBoxCount_ += 1;
		}
	}
}

void Dogs_Fight_PlayScene::BothViewDraw()
{
}

void Dogs_Fight_PlayScene::LeftViewDraw()
{
}

void Dogs_Fight_PlayScene::RightViewDraw()
{
}

void Dogs_Fight_PlayScene::UPSubViewDraw()
{
}

void Dogs_Fight_PlayScene::Release()
{
}

void Dogs_Fight_PlayScene::BoneSummons()
{
	const int boneCountNone = 0;
	const int boneCountMax = 1;
	const int addBoneCountAmount = 1;
	const float boneSummonsPosLimitMinX = 100.0f;
	const float boneSummonsPosLimitMaxX = 100.0f;
	const float boneSummonsPosLimitMinZ = 100.0f;
	const float boneSummonsPosLimitMaxZ = 100.0f;
	bool isCreateBone = false;     //骨を作ったかどうか

	if (boneCount_ == boneCountNone)
	{
		isCreateBone = true;
	}
	if (boneCount_ == boneCountMax)
	{
		isCreateBone = false;
	}

	if (isCreateBone)
	{
		for (int i = 0u; i < boneCountMax; i++)
		{
			pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::BONE, -boneSummonsPosLimitMinX, boneSummonsPosLimitMaxX, -boneSummonsPosLimitMinZ, boneSummonsPosLimitMaxZ);
			boneCount_ += addBoneCountAmount;
		}
	}
}

void Dogs_Fight_PlayScene::SetGameStop()
{
	isGameStop_ = true;
	Audio::Stop(hSound_[(int)BGM]);
	Audio::Stop(hSound_[(int)LASTBGM]);
	Audio::Stop(hSound_[(int)LASTBGM2]);
	Audio::Play(hSound_[(int)GAMESTOP], soundVolume_);
}
