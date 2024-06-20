//�C���N���[�h
#include "../Engine/Input.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Global.h"
#include "../ItemObject/Floor.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/Bone.h"
#include "../ItemObject/BoneSuck.h"
#include "../ItemObject/ItemObjectManager.h"
#include "../StageObject/Stage.h"
#include "../State/StateManager.h"
#include "../State/AttackPlayerState.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "../ImageManager.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"

AttackPlayer::AttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, attackPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1 }, stageHModel_{-1}, floorHModel_{-1}
    ,number_{0}, gameState_{ GAMESTATE::READY }, attackOrCollect_{ 0 }
    , pParent_{ nullptr }, pDogs_Walk_PlayScene_{ nullptr },pDogs_Fight_PlayScene_{nullptr}, pCollectPlayer_{nullptr}, pCollision_{nullptr}
    , pWoodBox_{ nullptr },pBoneSuck_{nullptr}, pText_{nullptr}, pStage_{nullptr}, pFloor_{nullptr}
    , pSceneManager_{nullptr}, pItemObjectManager_{nullptr}, pStateManager_{nullptr},pImageManager_{nullptr}
{
    pParent_ = _pParent;
}

AttackPlayer::~AttackPlayer()
{
}

void AttackPlayer::Initialize()
{
    //��INI�t�@�C������f�[�^�̃��[�h
    attackOrCollect_ = GetPrivateProfileInt("PLAYERPADID", "AttackOrCollect", 0, "Setting/PlayerSetting.ini");
    gameData_.walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
    //���T�E���h�f�[�^�̃��[�h
    std::string soundName;
    for (int i = 0; i < sizeof(soundAttackPlayerNames) / sizeof(soundAttackPlayerNames[initZeroInt]); i++)
    {
        soundName = soundFolderName + soundAttackPlayerNames[i] + soundModifierName;
        hSound_[i] = Audio::Load(soundName);
        assert(hSound_[i] >= 0);
    }
    //�����f���f�[�^�̃��[�h
    std::string modelName = modelFolderName + attackPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= 0);
    transform_.scale_ = { 0.4f,0.4f,0.4f };
    jumpData_.positionY_ = transform_.position_.y;
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
    {
        gameData_.FPS_ = 300;
    }
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
    pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
    pDogs_Fight_PlayScene_ = (Dogs_Fight_PlayScene*)FindObject(Dogs_Fight_PlaySceneName);
    pStage_ = (Stage*)FindObject(stageName);
    pFloor_ = (Floor*)FindObject(floorName);
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
    {
        pItemObjectManager_ = pDogs_Walk_PlayScene_->GetItemObjectManager();
    }
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
    {
        pItemObjectManager_ = pDogs_Fight_PlayScene_->GetItemObjectManager();
    }
    pStateManager_ = new StateManager(this);
    pStateManager_->AddState("WalkState", new AttackPlayerWalkState(pStateManager_));
    pStateManager_->AddState("WaitState", new AttackPlayerWaitState(pStateManager_));
    pStateManager_->AddState("RunState", new AttackPlayerRunState(pStateManager_));
    pStateManager_->AddState("JumpState", new AttackPlayerJumpState(pStateManager_));
    pStateManager_->AddState("StunState", new AttackPlayerStunState(pStateManager_));
    pStateManager_->ChangeState("WaitState");

    pText_ = new Text;
    pText_->Initialize();

    if (attackOrCollect_ == (int)PADIDSTATE::FIRST)
    {
        gameData_.padID_ = (int)PADIDSTATE::FIRST;
    }
    if (attackOrCollect_ == (int)PADIDSTATE::SECONDS)
    {
        gameData_.padID_ = (int)PADIDSTATE::SECONDS;
    }
    dirData_.vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(gameData_.padID_);
}

void AttackPlayer::Update()
{
    //�X�e�[�g�}�l�[�W���[�̍X�V
    pStateManager_->Update();

    switch (gameState_)
    {
    case GAMESTATE::READY:          UpdateReady();      break;
    case GAMESTATE::PLAY:           UpdatePlay();       break;
    case GAMESTATE::GAMEOVER:       UpdateGameOver();   break;
    }
}

void AttackPlayer::Draw()
{
    int drawScoreTextX = 30;
    int drawScoreTextY = 30;
    int drawScoreNumberX = 360;
    int drawScoreNumberY = 30;
    if(gameData_.padID_ == (int)PADIDSTATE::FIRST)
	{
        pText_->Draw(drawScoreTextX, drawScoreTextY, "AttackPlayer:Score=", true, false);
        pText_->Draw(drawScoreNumberX, drawScoreNumberY, gameData_.score_, true, false);
	}
    if (gameData_.padID_ == (int)PADIDSTATE::SECONDS)
    {
        pText_->Draw(drawScoreTextX, drawScoreTextY, "AttackPlayer:Score=", false, true);
        pText_->Draw(drawScoreNumberX, drawScoreNumberY, gameData_.score_, false, true);
    }

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void AttackPlayer::Release()
{
    SAFE_DELETE(pStateManager_);
}

void AttackPlayer::UpdateReady()
{
    PlayerCamera();
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //���f���ԍ����擾
    floorHModel_ = pFloor_->GetModelHandle();
    //�����̖@��(�n�ʂɒ���t��)
    stageDataDown.start = transform_.position_;  //���C�̔��ˈʒu
    stageDataDown.start.y = initZeroInt;
    XMStoreFloat3(&stageDataDown.dir, vecDown);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataDown); //���C�𔭎�
    wallData_.rayStageDistDown_ = stageDataDown.dist;
    if (stageDataDown.hit)
    {
        transform_.position_.y = -stageDataDown.dist + jumpData_.playerInitPosY_;
    }
    ++gameData_.timeCounter_;
    if (gameData_.timeCounter_ >= gameData_.timeLimit_)
    {
        gameState_ = GAMESTATE::PLAY;
        gameData_.timeCounter_ = initZeroInt;
    }
    jumpData_.positionY_ = transform_.position_.y;
}

void AttackPlayer::UpdatePlay()
{
    PlayerBase::UpdatePlay();
    if (Input::IsKeyDown(DIK_A))
    {
        if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
        {
            pDogs_Walk_PlayScene_->SetGameStop();
        }
        if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
        {
            pDogs_Fight_PlayScene_->SetGameStop();
        }
        gameState_ = GAMESTATE::GAMEOVER;
    }
    //���������̏���
    if (transform_.position_.y <= -gameData_.fallLimit_)
    {
        int revivalTime = 60;
        PlayerRevival();
        PlayerStun(revivalTime);
    }

    if (diveData_.isDive_ && !diveData_.isDived_)
    {
        ++diveData_.diveTime_;
        if (diveData_.diveTime_ <= diveData_.diveDuration_)
        {
            PlayerDivePower();
        }
        PlayerDive();
    }
    gameData_.scoreTimeCounter_++;
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
    {
        if (gameData_.scoreTimeCounter_ % gameData_.FPS_ == gameData_.scoreTimeCounterWait_)
        {
    		PlayerScore();
        }
    }
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
    {
        if (gameData_.scoreTimeCounter_ % gameData_.FPS_ == gameData_.scoreTimeCounterWait_ && boneData_.isBoneTatch_)
        {
            PlayerScore();
        }
    }

    PlayerCamera();
    PlayerFall();
    PlayerRayCast();
    PlayerKnockback();
    transform_.position_.y = jumpData_.positionY_;
    if (stunData_.isStun_)
    {
        stunData_.stunTimeCounter_++;
        if (stunData_.stunTimeCounter_ >= stunData_.stunLimit_)
        {
            stunData_.isStun_ = false;
            stunData_.isKnockBack_ = false;
            gameState_ = GAMESTATE::PLAY;
            stunData_.stunTimeCounter_ = initZeroInt;
        }
    }
    if (!stunData_.isStun_)
    {
        PlayerMove();
    }
    if (gameData_.score_ >= gameData_.scoreMax_)
    {
        if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
        {
            pDogs_Walk_PlayScene_->SetGameStop();
        }
        if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
        {
            pDogs_Fight_PlayScene_->SetGameStop();
        }
        gameState_ = GAMESTATE::GAMEOVER;
    }
    if (moveData_.isMove_ && !jumpData_.isJump_ && !moveData_.isRun_)
    {
        Audio::Play(hSound_[((int)SOUNDSTATE::WALK)], soundData_.soundVolume_);
    }
    if (!moveData_.isMove_ && !jumpData_.isJump_)
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, gameData_.padID_) && !jumpData_.isJump_ && moveData_.isMove_)
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::RUN)], soundData_.soundVolumeHalf_);
    }

    if (boneData_.isBoneTatch_)
    {
        if (boneData_.killTime_ > 0)
        {
            --boneData_.killTime_;
        }
    }

    if (boneData_.killTime_ <= 0 && boneData_.isBoneTatch_)
    {
        PlayerScore();
        if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
        {
            pDogs_Walk_PlayScene_->AddBoneCount(boneData_.decBoneCount_);
        }
        if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
        {
            pDogs_Fight_PlayScene_->AddBoneCount(boneData_.decBoneCount_);
        }
        boneData_.isBoneTatch_ = false;
        Audio::Stop(hSound_[((int)SOUNDSTATE::CollectBone)]);
        boneData_.killTime_ = boneData_.killTimeMax_;
    }
    IsMove();
    IsJump();
    IsRun();
    IsStun();
    IsDive();
}

void AttackPlayer::UpdateGameOver()
{
    pImageManager_ = Instantiate<ImageManager>(this);
    pImageManager_->SetMode((int)IMAGESTATE::GAMETITLE);
    if (gameData_.padID_ == (int)PADIDSTATE::FIRST)
    {
        Direct3D::SetIsChangeView(((int)Direct3D::VIEWSTATE::LEFTVIEW));
	}
    if (gameData_.padID_ == (int)PADIDSTATE::SECONDS)
    {
        Direct3D::SetIsChangeView(((int)Direct3D::VIEWSTATE::RIGHTVIEW));
    }
    if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::FIRST) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B, (int)PADIDSTATE::SECONDS))
    {
        //��INI�t�@�C���ւ̏�������
        WritePrivateProfileString("PLAYERSCORE", "AttackPlayerScore", std::to_string(gameData_.score_).c_str(), "Setting/PlayerSetting.ini");
        WritePrivateProfileString("PLAYERSCORE", "CollectPlayerScore", std::to_string(pCollectPlayer_->GetScore()).c_str(), "Setting/PlayerSetting.ini");
        pSceneManager_->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void AttackPlayer::PlayerWaitStateFunc()
{
    PlayerBase::PlayerWaitStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerWalkStateFunc()
{
    PlayerBase::PlayerWalkStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerRunStateFunc()
{
    PlayerBase::PlayerRunStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerJumpStateFunc()
{
    PlayerBase::PlayerJumpStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerStunStateFunc()
{
    PlayerBase::PlayerStunStateFunc();
    Model::SetAnimFrame(hModel_, animData_.startFrame_, animData_.endFrame_, animData_.animSpeed_);
}

void AttackPlayer::PlayerStun(int _timeLimit)
{
    PlayerBase::PlayerStun(_timeLimit);
}

void AttackPlayer::OnCollision(GameObject* _pTarget)
{
    std::vector<int> woodBoxs = {};
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
    {
        woodBoxs = pDogs_Walk_PlayScene_->GetWoodBoxs();
    }
    if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
    {
        woodBoxs = pDogs_Fight_PlayScene_->GetWoodBoxs();
    }
    woodBoxData_.woodBoxNumber_ = woodBoxName + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBoxData_.woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBoxData_.woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        woodBoxData_.dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos, vecUp));
        float angleRadians = acosf(woodBoxData_.dotProduct_);
        woodBoxData_.angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (woodBoxData_.angleDegrees_ <= woodBoxData_.angleDegreesMax_)
        {
            PlayerJumpPower();
            pWoodBox_->SetWoodBoxBreak();
            if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
            {
                pDogs_Walk_PlayScene_->AddWoodBoxCount(-woodBoxData_.woodBoxDeath_);
            }
            if (gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
            {
                pDogs_Fight_PlayScene_->AddWoodBoxCount(-woodBoxData_.woodBoxDeath_);
            }
        }
    }
    //WoodBox�Ƃ������O�����S�ẴI�u�W�F�N�g���Q��
    if (_pTarget->GetObjectName().find(woodBoxName) != std::string::npos)
    {
        if (woodBoxData_.angleDegrees_ > woodBoxData_.angleDegreesMax_)
        {
            transform_.position_ = moveData_.positionPrev_;
        }
    }
    if (_pTarget->GetObjectName() == boneName)
    {
        //Audio::Play(hSound_[((int)SOUNDSTATE::CollectBone)]);
        if (boneData_.killTime_ == boneData_.killTimeMax_ && gameData_.walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
        {
            Instantiate<BoneSuck>(this);

            pBoneSuck_ = (BoneSuck*)FindObject(boneSuckName);
            SetKillTime(boneData_.killTimeWait_);
            static int noDeathBoneSuck = 99999;
            pBoneSuck_->SetKillTime(noDeathBoneSuck);
            SetKillTime(noDeathBoneSuck);
            boneData_.isBoneTatch_ = true;
            _pTarget->KillMe();
        }
    }
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = initZeroInt;
    }

    if (_pTarget->GetObjectName() == collectPlayerName)
    {
        PlayerStun(stunData_.hitStopTime_);
        stunData_.isKnockBack_ = true;
        stunData_.vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pCollectPlayer_->GetVecPos());
        stunData_.vecKnockbackDirection_ = XMVector3Normalize(stunData_.vecKnockbackDirection_);
        if (boneData_.killTime_ < boneData_.killTimeMax_)
        {
            pBoneSuck_->SetKillTime(boneData_.killTimeWait_);
            SetKillTime(boneData_.killTimeWait_);
        }
    }
}

void AttackPlayer::PlayerScore()
{
    PlayerBase::PlayerScore();
}

void AttackPlayer::PlayerCamera()
{
    PlayerBase::PlayerCamera();
}

void AttackPlayer::PlayerFall()
{
    PlayerBase::PlayerFall();
}

void AttackPlayer::PlayerMove()
{
    dirData_.vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(gameData_.padID_);
    PlayerBase::PlayerMove();
    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, gameData_.padID_)))
    {
        XMVECTOR vecCam = {};
        moveData_.CamPositionVec_ = Camera::VecGetPosition(gameData_.padID_);
        vecCam = -(moveData_.CamPositionVec_ - Camera::VecGetTarget(gameData_.padID_));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = 0;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        dirData_.vecMove_ = vecCam;
    }

    if (Input::IsPadButton(XINPUT_GAMEPAD_A, gameData_.padID_) && !jumpData_.isJump_)
    {
        PlayerJumpPower();
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::JUMP)],soundData_.soundVolumeHalf_);
    }
}

void AttackPlayer::PlayerJump()
{

}

void AttackPlayer::PlayerJumpPower()
{
    PlayerBase::PlayerJumpPower();
}

void AttackPlayer::PlayerDive()
{
    PlayerBase::PlayerDive();
}

void AttackPlayer::PlayerDivePower()
{
    //�Ƃт��̏���
    PlayerBase::PlayerDivePower();
}

void AttackPlayer::PlayerKnockback()
{
    PlayerBase::PlayerKnockback();
}

void AttackPlayer::PlayerRayCast()
{
    float rayFloorDistUp = 0.0f;
    float rayStageBlockDistDown = 0.0f;
    float rayStageDistFront = 0.0f;
    float rayStageDistBack = 0.0f;
    float rayStageDistLeft = 0.0f;
    float rayStageDistRight = 0.0f;
    RayCastData floorDataUp;
    RayCastData floorDataDown;
    RayCastData stageDataDown;
    RayCastData stageDataFront;
    RayCastData stageDataBack;
    RayCastData stageDataLeft;
    RayCastData stageDataRight;                       //�v���C���[���n�ʂ���ǂ̂��炢����Ă����畂���Ă��锻��ɂ��邩
    stageHModel_ = pStage_->GetModelHandle();         //���f���ԍ����擾
    floorHModel_ = pFloor_->GetModelHandle();

    for (int i = 0;i < pItemObjectManager_->GetFloors().size(); i++)
    {
        //����̖@��(���蔲�����̂���)
        floorDataUp.start = transform_.position_;           //���C�̔��ˈʒu
        XMStoreFloat3(&floorDataUp.dir, vecUp);             //���C�̕���
        Model::RayCast(floorHModel_ + i, &floorDataUp);         //���C�𔭎�
        rayFloorDistUp = floorDataUp.dist;

        //�����̖@��(���蔲����)
        floorDataDown.start = transform_.position_;    //���C�̔��ˈʒu
        floorDataDown.start.y = 0;
        XMStoreFloat3(&floorDataDown.dir, vecDown);     //���C�̕���
        if (rayFloorDistUp == wallData_.distMax_)
        {
            Model::RayCast(floorHModel_ + i, &floorDataDown);  //���C�𔭎�
        }
        wallData_.rayFloorDistDown_ = floorDataDown.dist;
        if (wallData_.rayFloorDistDown_ + jumpData_.positionY_ <= moveData_.isFling_)
        {
            if (!jumpData_.isJump_)
            {
                floorData_.isOnFloor_ = true;
                diveData_.isDived_ = false;
                jumpData_.positionY_ = -wallData_.rayFloorDistDown_ + jumpData_.playerInitPosY_;
                jumpData_.positionTempY_ = jumpData_.positionY_;
                jumpData_.positionPrevY_ = jumpData_.positionTempY_;
            }
        }
        else
        {
            floorData_.isOnFloor_ = false;
        }

    }
    //�����̖@��(�n�ʂɒ���t��)
    stageDataDown.start = transform_.position_;   //���C�̔��ˈʒu
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataDown); //���C�𔭎�
    wallData_.rayStageDistDown_ = stageDataDown.dist;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    if (wallData_.rayStageDistDown_ + jumpData_.positionY_ <= moveData_.isFling_)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (!jumpData_.isJump_ && !floorData_.isOnFloor_)
        {
            //�n�ʂɒ���t��
            diveData_.isDived_ = false;
            jumpData_.positionY_ = -wallData_.rayStageDistDown_ + jumpData_.playerInitPosY_;
            jumpData_.positionTempY_ = jumpData_.positionY_;
            jumpData_.positionPrevY_ = jumpData_.positionTempY_;
        }
    }
    //���O�̖@��(�ǂ̓����蔻��)
    stageDataFront.start = transform_.position_;      //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataFront.dir, vecFrontUp);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataFront);    //���C�𔭎�
    rayStageDistFront = stageDataFront.dist;
    if (rayStageDistFront <= wallData_.inTheWall_)
    {
        transform_.position_.z = moveData_.positionPrev_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    stageDataBack.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataBack.dir, vecBackUp);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataBack);     //���C�𔭎�
    rayStageDistBack = stageDataBack.dist;
    if (rayStageDistBack <= wallData_.inTheWall_)
    {
        transform_.position_.z = moveData_.positionPrev_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    stageDataLeft.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataLeft.dir, vecLeftUp);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataLeft);     //���C�𔭎�
    rayStageDistLeft = stageDataLeft.dist;
    if (rayStageDistLeft <= wallData_.inTheWall_)
    {
        transform_.position_.x = moveData_.positionPrev_.x;
    }
    //���E�̖@��(�ǂ̓����蔻��)
    stageDataRight.start = transform_.position_;      //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataRight.dir, vecRightUp);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataRight);    //���C�𔭎�
    rayStageDistRight = stageDataRight.dist;
    if (rayStageDistRight <= wallData_.inTheWall_)
    {
        transform_.position_.x = moveData_.positionPrev_.x;
    }
    moveData_.positionPrev_ = transform_.position_;
}

void AttackPlayer::PlayerRevival()
{
    PlayerBase::PlayerRevival();
}

void AttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    PlayerBase::SetKnockback(_vecKnockbackDirection, _knockbackSpeed);
}

void AttackPlayer::IsMove()
{
    PlayerBase::IsMove();
}

void AttackPlayer::IsJump()
{
    PlayerBase::IsJump();
}

void AttackPlayer::IsRun()
{
    PlayerBase::IsRun();
}

void AttackPlayer::IsStun()
{
    PlayerBase::IsStun();
}

void AttackPlayer::IsDive()
{
    PlayerBase::IsDive();
}
