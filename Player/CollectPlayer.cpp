//�C���N���[�h
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"
#include "../Engine/VFX.h"
#include "../Engine/Global.h"
#include "../ItemObject/Floor.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/Bone.h"
#include "../ItemObject/ItemObjectManager.h"
#include "../StageObject/Stage.h"
#include "../StageObject/StageBlock.h"
#include "../State/StateManager.h"
#include "../State/PlayerState.h"
#include "CollectPlayer.h"
#include "AttackPlayer.h"

CollectPlayer::CollectPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, collectPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1,-1 }, stageBlockHModel_{ -1 }, stageHModel_{ -1 }, floorHModel_{ -1 }
    , decBoneCount_{ -1 }, isBoneDeath_{ false }, isBoneTatch_{ false }, number_{ 0 }, killTime_{ 9999 }, killTimeWait_{ 30 }, killTimeMax_{ 9999 }
    , gameState_{GAMESTATE::READY}
    , pParent_{ nullptr }, pPlayScene_{ nullptr }, pAttackPlayer_{ nullptr }, pCollision_{ nullptr }
    , pWoodBox_{ nullptr }, pText_{ nullptr }, pStage_{ nullptr }, pStageBlock_{ nullptr }, pFloor_{ nullptr }, pSceneManager_{ nullptr },pItemObjectManager_{nullptr},pStateManager_{nullptr}
{
    pParent_ = _pParent;
    //��UI�Ɋւ�����N���X�����o�ϐ�
    drawScoreTextX_ = 30;
    drawScoreTextY_ = 60;
    drawScoreNumberX_ = 360;
    drawScoreNumberY_ = 60;
    //���Q�[�����o�Ɋւ�����N���X�����o�ϐ�
    FPS_ = 600;
    timeCounter_ = 0;
    timeLimit_ = 60;
    fallLimit_ = 100.0f;
    score_ = 0;
    scoreAmount_ = 10;
    scoreMax_ = 150;
    padID_ = 1;
    playerInitPosY_ = 0.6f;
    //���T�E���h�Ɋւ�����N���X�����o�ϐ�
    soundVolume_ = 0.5f;
    soundVolumeHalf_ = soundVolume_ / 2;
    //�����W���v���C���[�ړ��Ɋւ�����N���X�����o�ϐ�
    CamPositionVec_ = {};
    positionPrev_ = { 0.0f,0.0f,0.0f };
    controllerMoveSpeed_ = { 0.3f,0.0f,0.3f };
    positionY_ = 0.0f;
    isDash_ = false;
    isFling_ = 1.0f;
    //�������ς��Ɋւ�����N���X�����o�ϐ�
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(collectPlayerNumber);
    angle_ = 0.0f;
    //�����W���v���C���[�W�����v�Ɋւ�����N���X�����o�ϐ�
    gravity_ = 0.007f;
    positionTempY_ = 0.0f;
    positionPrevY_ = 0.0f;
    jumpPower_ = 0.3f;
    isJump_ = false;
    //����т��Ɋւ��郁���o�ϐ�
    divePower_ = 0.1f;
    diveSpeed_ = 0.6f;
    isDive_ = false;
    isDived_ = false;
    diveTime_ = 0;
    diveDuration_ = 1;
    diveTimeWait_ = 30;
    //�����蔲�����Ɋւ�����N���X�����o�ϐ�
    isOnFloor_ = false;
    //���ؔ��Ɋւ�����N���X�����o�ϐ�
    woodBoxNumber_ = "";
    dotProduct_ = 0.0f;
    angleDegrees_ = 0.0f;
    angleDegreesMax_ = 80.0f;
    //���X�^���Ɋւ�����N���X�����o�ϐ�
    stunTimeCounter_ = 0;
    stunLimit_ = 0;
    hitStopTime_ = 10;
    getUpTime_ = 30;
    knockbackSpeed_ = 0.3f;
    isStun_ = false;
    isKnockBack_ = false;
    //���ǔ���Ɋւ�����N���X�����o�ϐ�
    distMax_ = 99999.0f;
    inTheWall_ = 1.5f;
    rayFloorDistDown_ = 0.0f;
    rayStageDistDown_ = 0.0f;
}

CollectPlayer::~CollectPlayer()
{
}

void CollectPlayer::Initialize()
{
    //���T�E���h�f�[�^�̃��[�h
    std::string soundName;
    for (int i = initZeroInt; i < sizeof(soundCollectPlayerNames) / sizeof(soundCollectPlayerNames[initZeroInt]); i++)
    {
        soundName = soundFolderName + soundCollectPlayerNames[i] + soundModifierName;
        hSound_[i] = Audio::Load(soundName);
        assert(hSound_[i] >= initZeroInt);
    }
    //�����f���f�[�^�̃��[�h
    std::string modelName = modelFolderName + collectPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    transform_.scale_ = { 0.4f,0.4f,0.4f};
    positionY_ = transform_.position_.y;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pStage_ = (Stage*)FindObject(stageName);      //�X�e�[�W�I�u�W�F�N�g
    pStageBlock_ = (StageBlock*)FindObject(stageBlockName);
    pFloor_ = (Floor*)FindObject(floorName);
    pItemObjectManager_ = pPlayScene_->GetItemObjectManager();
    pStateManager_ = new StateManager(this);

    pStateManager_->AddState("WaitState", new PlayerWaitState(pStateManager_));
    pStateManager_->AddState("WalkState", new PlayerWalkState(pStateManager_));
    pStateManager_->AddState("RunState", new PlayerRunState(pStateManager_));
    pStateManager_->AddState("JumpState", new PlayerJumpState(pStateManager_));
    pStateManager_->AddState("StunState", new PlayerStunState(pStateManager_));
    pStateManager_->ChangeState("WaitState");
    pText_ = new Text;
    pText_->Initialize();
}

void CollectPlayer::Update()
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

void CollectPlayer::Draw()
{
    pText_->Draw(drawScoreTextX_, drawScoreTextY_, "CollectPlayer:Score=");
    pText_->Draw(drawScoreNumberX_, drawScoreNumberY_, score_);

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void CollectPlayer::Release()
{
    SAFE_DELETE(pStateManager_);
}

void CollectPlayer::UpdateReady()
{
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //���f���ԍ����擾
    floorHModel_ = pFloor_->GetModelHandle();
    //�����̖@��(�n�ʂɒ���t��)
    stageDataDown.start = transform_.position_;  //���C�̔��ˈʒu
    stageDataDown.start.y = initZeroInt;
    XMStoreFloat3(&stageDataDown.dir, vecDown);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataDown); //���C�𔭎�
    rayStageDistDown_ = stageDataDown.dist;
    if (stageDataDown.hit)
    {
        transform_.position_.y = -stageDataDown.dist + playerInitPosY_;
    }
    ++timeCounter_;
    if (timeCounter_ >= timeLimit_)
    {
        gameState_ = GAMESTATE::PLAY;
        timeCounter_ = initZeroInt;
    }
    positionY_ = transform_.position_.y;
}

void CollectPlayer::UpdatePlay()
{
    PlayerBase::UpdatePlay();
    if (Input::IsKeyDown(DIK_D))
    {
        pPlayScene_->SetGameStop();
        gameState_ = GAMESTATE::GAMEOVER;
    }
    //���������̏���
    if (transform_.position_.y <= -fallLimit_)
    {
        int revivalTime = 60;
        PlayerRevival();
        PlayerStun(revivalTime);
    }

    if (isDive_ && !isDived_)
    {
        ++diveTime_;
        if (diveTime_ <= diveDuration_)
        {
            PlayerDivePower();
        }
        PlayerDive();
    }

    PlayerFall();
    PlayerJump();
    PlayerRayCast();
    PlayerKnockback();
    transform_.position_.y = positionY_;
    if (isStun_)
    {
        stunTimeCounter_++;
        if (stunTimeCounter_ >= stunLimit_)
        {
            gameState_ = GAMESTATE::PLAY;
            isStun_ = false;
            isKnockBack_ = false;
            stunTimeCounter_ = initZeroInt;
        }
    }
    if (!isStun_)
    {
        PlayerMove();
    }
    if (score_ >= scoreMax_)
    {
        pPlayScene_->SetGameStop();
        gameState_ = GAMESTATE::GAMEOVER;
    }
    if (IsMoving() && !isJump_ && !isDash_)
    {
        Audio::Play(hSound_[((int)SOUNDSTATE::WALK)], soundVolume_);
    }
    if (!IsMoving() && !isJump_)
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, padID_) && !isJump_ && IsMoving())
    {
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::RUN)], soundVolumeHalf_);
        isDash_ = true;
    }
    else
    {
        isDash_ = false;
    }
    if (isJump_)
    {
    }
    if (isStun_)
    {
    }
    if (isBoneTatch_)
    {
        if (killTime_ > initZeroInt)
        {
            --killTime_;
        }
    }

    if (killTime_ <= initZeroInt && isBoneTatch_)
    {
        score_ += scoreAmount_;
        pPlayScene_->AddBoneCount(decBoneCount_);
        isBoneDeath_ = true;
        isBoneTatch_ = false;
        Audio::Stop(hSound_[((int)SOUNDSTATE::CollectBone)]);
        killTime_ = killTimeMax_;
    }
}

void CollectPlayer::UpdateGameOver()
{
    Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFTVIEW);
    if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, collectPlayerNumber))
    {
        pSceneManager_->ChangeScene(SCENE_ID_GAMEOVER);
        PlayerScore_[collectPlayerNumber] = this->GetScore();
        PlayerScore_[attackPlayerNumber] = pAttackPlayer_->GetScore();
    }
}

void CollectPlayer::PlayerWaitStateFunc()
{
    Model::SetAnimFrame(hModel_, 0, 0, 1.0f);
}

void CollectPlayer::PlayerWalkStateFunc()
{
    Model::SetAnimFrame(hModel_, 20, 60, 0.5f);
}

void CollectPlayer::PlayerRunStateFunc()
{
    Model::SetAnimFrame(hModel_, 80, 120, 0.5f);
}

void CollectPlayer::PlayerJumpStateFunc()
{
    Model::SetAnimFrame(hModel_, 120, 120, 1.0f);
}

void CollectPlayer::PlayerStunStateFunc()
{
    Model::SetAnimFrame(hModel_, 140, 200, 0.5f);
}

void CollectPlayer::PlayerStun(int _timeLimit)
{
    PlayerBase::PlayerStun(_timeLimit);
    Audio::Play(hSound_[((int)SOUNDSTATE::STUN)], soundVolume_);
}

void CollectPlayer::OnCollision(GameObject* _pTarget)
{
    std::vector<int> woodBoxs = pPlayScene_->GetWoodBoxs();
    woodBoxNumber_ = woodBoxName + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos, vecUp));
        float angleRadians = acosf(dotProduct_);
        angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (angleDegrees_ <= angleDegreesMax_)
        {
            PlayerJumpPower();
            pWoodBox_->SetWoodBoxBreak();
            pPlayScene_->AddWoodBoxCount(-woodBoxDeath_);

        }
    }
    //WoodBox�Ƃ������O�����S�ẴI�u�W�F�N�g���Q��
    if (_pTarget->GetObjectName().find(woodBoxName) != std::string::npos)
    {
        if (angleDegrees_ > angleDegreesMax_)
        {
            transform_.position_ = positionPrev_;
        }
    }

    if (_pTarget->GetObjectName() == boneName && killTime_ == killTimeMax_)
    {
        SetKillTime(killTimeWait_);
        isBoneTatch_ = true;
        Audio::Play(hSound_[((int)SOUNDSTATE::CollectBone)]);
    }
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = initZeroInt;
    }

    if (_pTarget->GetObjectName() == attackPlayerName)
    {
        PlayerStun(hitStopTime_);
        isKnockBack_ = true;
        vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pAttackPlayer_->GetVecPos());
        vecKnockbackDirection_ = XMVector3Normalize(vecKnockbackDirection_);
    }
}

void CollectPlayer::PlayerFall()
{
    PlayerBase::PlayerFall();
}

void CollectPlayer::PlayerMove()
{
    vecDirection_ = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(collectPlayerNumber);
    PlayerBase::PlayerMove();
    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, padID_)))
    {
        XMVECTOR vecCam = {};
        CamPositionVec_ = Camera::VecGetPosition(collectPlayerNumber);
        vecCam = -(CamPositionVec_ - Camera::VecGetTarget(collectPlayerNumber));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = 0;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        vecMove_ = vecCam;
    }

    if (Input::IsPadButton(XINPUT_GAMEPAD_A, padID_) && !isJump_)
    {
        PlayerJumpPower();
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::JUMP)], soundVolumeHalf_);
    }
}

void CollectPlayer::PlayerJump()
{

}

void CollectPlayer::PlayerJumpPower()
{
    PlayerBase::PlayerJumpPower();
}

void CollectPlayer::PlayerDive()
{
    PlayerBase::PlayerDive();
}

void CollectPlayer::PlayerDivePower()
{
    //�_�C�u�̏���
    PlayerBase::PlayerDivePower();
}

void CollectPlayer::PlayerKnockback()
{
    PlayerBase::PlayerKnockback();
}

void CollectPlayer::PlayerRayCast()
{
    float rayFloorDistUp = 0.0f;
    float rayStageBlockDistDown = 0.0f;
    float rayStageDistFront = 0.0f;
    float rayStageDistBack = 0.0f;
    float rayStageDistLeft = 0.0f;
    float rayStageDistRight = 0.0f;
    RayCastData floorDataUp;
    RayCastData floorDataDown;
    RayCastData stageBlockDataDown;
    RayCastData stageDataDown;
    RayCastData stageDataFront;
    RayCastData stageDataBack;
    RayCastData stageDataLeft;
    RayCastData stageDataRight;                 //�v���C���[���n�ʂ���ǂ̂��炢����Ă����畂���Ă��锻��ɂ��邩
    stageHModel_ = pStage_->GetModelHandle();         //���f���ԍ����擾
    floorHModel_ = pFloor_->GetModelHandle();

    for (int i = 0; i < pItemObjectManager_->GetFloors().size(); i++)
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
        if (floorDataUp.dist == distMax_)
        {
            Model::RayCast(floorHModel_ + i, &floorDataDown);  //���C�𔭎�
        }
        rayFloorDistDown_ = floorDataDown.dist;
        if (rayFloorDistDown_ + positionY_ <= isFling_)
        {
            if (!isJump_)
            {
                isOnFloor_ = true;
                isDived_ = false;
                positionY_ = -floorDataDown.dist + playerInitPosY_;
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
            }
        }
        else
        {
            isOnFloor_ = false;
        }

    }

    //�����̖@��(���ɒ���t��)
    stageDataDown.start = transform_.position_;     //���C�̔��ˈʒu
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataDown);   //���C�𔭎�
    rayStageDistDown_ = stageDataDown.dist;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    if (rayStageDistDown_ + positionY_ <= isFling_)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (!isJump_ && !isOnFloor_)
        {
            //�n�ʂɒ���t��
            isDived_ = false;
            positionY_ = -stageDataDown.dist + playerInitPosY_;
            positionTempY_ = positionY_;
            positionPrevY_ = positionTempY_;
        }
    }
    else if (!isOnFloor_)
    {
        isJump_ = true;
    }
    //���O�̖@��(�ǂ̓����蔻��)
    stageDataFront.start = transform_.position_;      //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataFront.dir, vecFrontUp);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataFront);    //���C�𔭎�
    rayStageDistFront = stageDataFront.dist;
    if (rayStageDistFront <= inTheWall_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    stageDataBack.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataBack.dir, vecBackUp);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataBack);     //���C�𔭎�
    rayStageDistBack = stageDataBack.dist;
    if (rayStageDistBack <= inTheWall_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    stageDataLeft.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataLeft.dir, vecLeftUp);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataLeft);     //���C�𔭎�
    rayStageDistLeft = stageDataLeft.dist;
    if (rayStageDistLeft <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    //���E�̖@��(�ǂ̓����蔻��)
    stageDataRight.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataRight.dir, vecRightUp);    //���C�̕���
    Model::RayCast(stageHModel_, &stageDataRight);     //���C�𔭎�
    rayStageDistRight = stageDataRight.dist;
    if (rayStageDistRight <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    positionPrev_ = transform_.position_;
}

void CollectPlayer::PlayerRevival()
{
    PlayerBase::PlayerRevival();
}

void CollectPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

bool CollectPlayer::IsMoving()
{
    return (transform_.position_.x != positionPrev_.x || transform_.position_.z != positionPrev_.z);
}