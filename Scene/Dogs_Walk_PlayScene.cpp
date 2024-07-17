//インクルード
#include <random>
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "../Player/AIPlayer.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/FrameBox.h"
#include "../ItemObject/BoneSuck.h"
#include "../ItemObject/Bone.h"
#include "../StageObject/StageObjectManager.h"
#include "../ImageManager.h"
#include "Dogs_Walk_PlayScene.h"

Dogs_Walk_PlayScene::Dogs_Walk_PlayScene(GameObject* _pParent)
	:GameObject(_pParent, Dogs_Walk_PlaySceneName),attackOrCollect_{0},attackOrCollectInverse_{0}, stageBlockNum_{3}, lengthRecedes_{5}, degreesMin_{0.0f}
	, degreesMax_{-88.0f}, degreesToRadians_{3.14f / 180.0f}, vecLengthRecedes_{1.0f}, vecLengthApproach_{1.0f}
	, woodBoxCountMax_{5}, changeScore_{100}, hSound_{-1,-1,-1}
	, randomValue_{0}, soundVolume_{0.05f,}, soundVolumeHalf_{soundVolume_ / 2}, length_{30}, boneCount_{0}
	, collectPlayerPosition_{}, collectPlayerDirection_{},boneFrontPosition_{2.0f}, woodBoxCount_{0}
	, attackPlayerPosition_{}, attackPlayerDirection_{},woodBoxFrontPosition_{10.0f},time_{1.0f}, isGameStop_{false}
	,pSceneManager_{nullptr}, pAttackPlayer_{nullptr}, pCollectPlayer_{nullptr}, pItemObjectManager_{nullptr}, pStageObjectManager_{nullptr}
	,pImageManager_{nullptr},pAIPlayer_{nullptr}
{

}

void Dogs_Walk_PlayScene::Initialize()
{
	//▼INIファイルからデータのロード
	attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
	attackOrCollectInverse_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollectInverse", 0, "Setting/PlayerSetting.ini");
	//▼サウンドデータのロード
	std::string soundName;
	for (int i = 0u; i < sizeof(soundDogs_Walk_PlaySceneNames) / sizeof(soundDogs_Walk_PlaySceneNames[initZeroInt]); i++)
	{
		soundName = soundFolderName + soundDogs_Walk_PlaySceneNames[i] + soundModifierName;
		hSound_[i] = Audio::Load(soundName);
		assert(hSound_[i] >= 0u);
	}
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pItemObjectManager_ = new ItemObjectManager(this);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::STAGE);

	float stageBlockSummonsPosLimitMinX = 100.0f;
	float stageBlockSummonsPosLimitMaxX = 100.0f;
	float stageBlockSummonsPosLimitMinZ = 100.0f;
	float stageBlockSummonsPosLimitMaxZ = 100.0f;

	for (int i = 0u; i < stageBlockNum_; i++)
	{
		pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::STAGEBLOCK, -stageBlockSummonsPosLimitMinX, stageBlockSummonsPosLimitMaxX, -stageBlockSummonsPosLimitMinZ, stageBlockSummonsPosLimitMaxZ);
	}
	floorPosition_[0].position_ = { 30.0f,0.8f,3.0f };
	floorPosition_[1].position_ = { -70.0f,0.5f,50.0f };
	floorPosition_[2].position_ = { -45.0f, 0.3f,-45.0f };
	XMFLOAT3 scale		   = { 3.0f,1.0f,3.0f };
	XMFLOAT3 DefaultData[2] = { XMFLOAT3(0.0f,0.0f,0.0f)	//0で初期化
							   ,XMFLOAT3(1.0f,1.0f,1.0f) };	//1で初期化
	XMFLOAT3 WoodBox[3]		= {XMFLOAT3(30.0f,-5.0f,40.0f)
							  ,XMFLOAT3(20.0f,-10.0f,40.0f)
							  ,XMFLOAT3(10.0f,-20.0f,40.0f) };
	XMFLOAT3 FrameBox = { XMFLOAT3(5.0f,5.0f,5.0f) };

	pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::FLOOR, floorPosition_[2].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));
	pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::FLOOR, floorPosition_[1].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));
	pAttackPlayer_ = Instantiate<AttackPlayer>(this);
	camVec_[attackOrCollect_] = XMFLOAT3(0, 5, -10);
	pCollectPlayer_ = Instantiate<CollectPlayer>(this);
	pAIPlayer_ = Instantiate<AIPlayer>(this);
	camVec_[attackOrCollectInverse_] = XMFLOAT3(0, 5, -10);
	pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::FRAMEBOX,DefaultData[0], DefaultData[1], FrameBox);
	pAttackPlayer_->SetCollectPlayer(pCollectPlayer_);
	pAttackPlayer_->SetAIPlayer(pAIPlayer_);
	pCollectPlayer_->SetAttackPlayer(pAttackPlayer_);
	pCollectPlayer_->SetAIPlayer(pAIPlayer_);
	pAIPlayer_->SetCollectPlayer(pCollectPlayer_);
	pAIPlayer_->SetAttackPlayer(pAttackPlayer_);
	playerFirstPos_[(int)PLAYERNUMSTATE::ATTACKPLAYER] = XMFLOAT3(0, 0, 8);
	playerFirstPos_[(int)PLAYERNUMSTATE::COLLECTPLAYER] = XMFLOAT3(0, 0, -8);
	playerFirstPos_[(int)PLAYERNUMSTATE::AIPLAYER] = XMFLOAT3(0, 0, 0);
	pAttackPlayer_->SetPosition(playerFirstPos_[(int)PLAYERNUMSTATE::ATTACKPLAYER]);
	pCollectPlayer_->SetPosition(playerFirstPos_[(int)PLAYERNUMSTATE::COLLECTPLAYER]);
	pAIPlayer_->SetPosition(playerFirstPos_[(int)PLAYERNUMSTATE::AIPLAYER]);
	pAttackPlayer_->SetImageSecInit();
	pCollectPlayer_->SetImageSecInit();

	//乱数生成器の設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 2);

	//1から2までのランダムな値の作成
	randomValue_ = dis(gen);
	pImageManager_ = Instantiate<ImageManager>(this);
	pImageManager_->SetMode((int)IMAGESTATE::TIMEGAUGE);
	pImageManager_->SetGaugeMode((int)GAUGESTATE::WALK);
	pImageManager_->SecInit();
}

void Dogs_Walk_PlayScene::Update()
{
	pImageManager_->AddGaugeScale(0.08f);
	if((!isGameStop_ && pAttackPlayer_->GetScore() >= changeScore_) || (!isGameStop_ && pCollectPlayer_->GetScore() >= changeScore_))
	{
		Audio::Stop(hSound_[(int)SOUNDSTATE::BGM]);
		Audio::Play(hSound_[randomValue_],soundVolume_);
	}
	if((!isGameStop_ && pAttackPlayer_->GetScore() < changeScore_) || (!isGameStop_ && pCollectPlayer_->GetScore() < changeScore_))
	{
		Audio::Play(hSound_[(int)SOUNDSTATE::BGM], soundVolumeHalf_);
	}
	//木箱が邪魔側の犬の前にくるための計算
	attackPlayerPosition_ = pAttackPlayer_->GetPosition();
	attackPlayerDirection_ = XMLoadFloat3(&attackPlayerPosition_) - Camera::VecGetPosition(pAttackPlayer_->GetPadID());
	attackPlayerDirection_ = XMVectorSetY(attackPlayerDirection_, 0.0f);	//y座標の初期化
	attackPlayerDirection_ = XMVector3Normalize(attackPlayerDirection_);
	attackPlayerPosition_.x += woodBoxFrontPosition_ * XMVectorGetX(attackPlayerDirection_);
	attackPlayerPosition_.y += -2.0f;
	attackPlayerPosition_.z += woodBoxFrontPosition_ * XMVectorGetZ(attackPlayerDirection_);
	//骨が収集側の犬の口にくるための計算
	collectPlayerPosition_ = pCollectPlayer_->GetPosition();
	collectPlayerDirection_ = XMLoadFloat3(&collectPlayerPosition_) - Camera::VecGetPosition(pCollectPlayer_->GetPadID());
	collectPlayerDirection_ = XMVectorSetY(collectPlayerDirection_, 0);	//y座標の初期化
	collectPlayerDirection_ = XMVector3Normalize(collectPlayerDirection_);
	collectPlayerPosition_.x += boneFrontPosition_ * XMVectorGetX(collectPlayerDirection_);
	collectPlayerPosition_.z += boneFrontPosition_ * XMVectorGetZ(collectPlayerDirection_);
	static bool isPause = false;
	if (Input::IsKeyDown(DIK_P))
	{
		isPause = !isPause;
	}
	HideCursor();
	BoneSummons();
	if (woodBoxCount_ <= woodBoxCountMax_)
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_Y,pAttackPlayer_->GetPadID()) && pAttackPlayer_->GetIsJump())
		{
			XMFLOAT3 woodBoxRotate = {};
			woodBoxRotate.y = pAttackPlayer_->GetAngle();
			pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::WOODBOX, attackPlayerPosition_, woodBoxRotate, XMFLOAT3(0.5f, 0.5f, 0.5f));
			woodBoxCount_ += 1;
		}
	}
}

void Dogs_Walk_PlayScene::BothViewDraw()
{
}

void Dogs_Walk_PlayScene::LeftViewDraw()
{
}

void Dogs_Walk_PlayScene::RightViewDraw()
{

}

void Dogs_Walk_PlayScene::UPSubViewDraw()
{
}

void Dogs_Walk_PlayScene::Release()
{
}

void Dogs_Walk_PlayScene::BoneSummons()
{
	int boneCountNone = 0;
	int boneCountMax = 3;
	int addBoneCountAmount = 1;
	float boneSummonsPosLimitMinX = 100.0f;
	float boneSummonsPosLimitMaxX = 100.0f;
	float boneSummonsPosLimitMinZ = 100.0f;
	float boneSummonsPosLimitMaxZ = 100.0f;
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

void Dogs_Walk_PlayScene::SetGameStop()
{
	isGameStop_ = true;
	Audio::Stop(hSound_[(int)SOUNDSTATE::BGM]);
	Audio::Stop(hSound_[(int)SOUNDSTATE::LASTBGM]);
	Audio::Stop(hSound_[(int)SOUNDSTATE::LASTBGM2]);
	Audio::Play(hSound_[(int)SOUNDSTATE::GAMESTOP],soundVolume_);
}
