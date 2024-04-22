//�C���N���[�h
#include "../Engine/SceneManager.h"
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
#include "../ItemObject/ItemObjectManager.h"
#include "../StageObject/Stage.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"

AttackPlayer::AttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, attackPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1 }, stageHModel_{0}, floorHModel_{0}
    , number_{ 0 },  scoreTimeCounter_{ 0 }, scoreTimeCounterWait_{ 1 }, vecKnockbackDirection_ {}, playerState_{ PLAYERSTATE::WAIT }, playerStatePrev_{ PLAYERSTATE::WAIT }, gameState_{ GAMESTATE::READY }
    , pParent_{ nullptr }, pPlayScene_{ nullptr }, pCollectPlayer_{ nullptr }, pCollision_{ nullptr }
    , pWoodBox_{ nullptr }, pText_{ nullptr }, pStage_{ nullptr }, pFloor_{ nullptr }, pSceneManager_{ nullptr },pItemObjectManager_{nullptr}
{
    pParent_ = _pParent;
    //��UI�Ɋւ�����N���X�����o�ϐ�
    drawScoreTextX_ = 30;
    drawScoreTextY_ = 30;
    drawScoreNumberX_ = 360;
    drawScoreNumberY_ = 30;
    //���Q�[�����o�Ɋւ�����N���X�����o�ϐ�
    FPS_ = 60 * 10;
    timeCounter_ = 0;
    timeLimit_ = 60;
    fallLimit_ = 100.0f;
    score_ = 0;
    scoreAmount_ = 10;
    scoreMax_ = 150;
    padID_ = 0;
    playerInitPosY_ = 0.6f;
    //���T�E���h�Ɋւ�����N���X�����o�ϐ�
    soundVolume_ = 0.5f;
    soundVolumeHalf_ = soundVolume_ / 2;
    //���ז����v���C���[�ړ��Ɋւ�����N���X�����o�ϐ�
    CamPositionVec_ = {};
    positionPrev_ = { 0.0f,0.0f,0.0f };
    controllerMoveSpeed_ = 0.3f;
    mouseMoveSpeed_ = 0.3f;
    positionY_ = 0.0f;
    isDash_ = false;
    isFling_ = 1.0f;
    pi_ = 3.14;
    halfPi_ = pi_ / 2.0f;
    dashSpeed_ = 0.5f;
    walkSpeed_ = 0.4f;
    //�������ς��Ɋւ�����N���X�����o�ϐ�
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecLength_ = { 0.0f,0.0f,0.0f,0.0f };
    vecDot_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    length_ = 0.0f;
    dot_ = 0.0f;
    angle_ = 0.0f;
    //���ז����v���C���[�W�����v�Ɋւ�����N���X�����o�ϐ�
    gravity_ = 0.007f;
    positionTempY_ = 0.0f;
    positionPrevY_ = 0.0f;
    jumpPower_ = 0.3f;
    isJump_ = false;
    //����т��Ɋւ�����N���X�����o�ϐ�
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
    outerWallPosFront_ = 99.0f;
    outerWallPosBack_ = -99.0f;
    outerWallPosLeft_ = 99.0f;
    outerWallPosRight_ = -99.0f;
    rayStageDistDown_ = 0.0f;
    rayFloorDistDown_ = 0.0f;
    rayFloorDistUp_ = 0.0f;
    rayStageDistFront_ = 0.0f;
    rayStageDistBack_ = 0.0f;
    rayStageDistLeft_ = 0.0f;
    rayStageDistRight_ = 0.0f;
}

AttackPlayer::~AttackPlayer()
{
}

void AttackPlayer::Initialize()
{
    //���T�E���h�f�[�^�̃��[�h
    std::string soundName;
    for (int i = initZeroInt; i < sizeof(soundAttackPlayerNames) / sizeof(soundAttackPlayerNames[initZeroInt]); i++)
    {
        soundName = soundFolderName + soundAttackPlayerNames[i] + soundModifierName;
        hSound_[i] = Audio::Load(soundName);
        assert(hSound_[i] >= initZeroInt);
    }
    //�����f���f�[�^�̃��[�h
    std::string modelName = modelFolderName + attackPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    transform_.scale_ = { 0.4f,0.4f,0.4f };
    positionY_ = transform_.position_.y;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pStage_ = (Stage*)FindObject(stageName);
    pFloor_ = (Floor*)FindObject(floorName);
    pItemObjectManager_ = pPlayScene_->GetItemObjectManager();
    pText_ = new Text;
    pText_->Initialize();
}

void AttackPlayer::Update()
{
    switch (gameState_)
    {
    case GAMESTATE::READY:          UpdateReady();      break;
    case GAMESTATE::PLAY:           UpdatePlay();       break;
    case GAMESTATE::GAMEOVER:       UpdateGameOver();   break;
    }
}

void AttackPlayer::Draw()
{
    pText_->Draw(drawScoreTextX_, drawScoreTextY_, "AttackPlayer:Score=");
    pText_->Draw(drawScoreNumberX_, drawScoreNumberY_, score_);

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void AttackPlayer::Release()
{

}

void AttackPlayer::UpdateReady()
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

void AttackPlayer::UpdatePlay()
{
    if (Input::IsKeyDown(DIK_A))
    {
        pPlayScene_->SetGameStop();
        gameState_ = GAMESTATE::GAMEOVER;
    }
    //���������̏���
    if (transform_.position_.y <= -fallLimit_)
    {
        transform_.position_ = initZeroXMFLOAT3;
    }

    if(Input::GetPadTrrigerR(padID_))
    {
        isDive_ = true;
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
    if (playerStatePrev_ != playerState_)
    {
        switch (playerState_)
        {
        case PLAYERSTATE::WAIT:       Model::SetAnimFrame(hModel_, 0, 0, 1.0f); break;
        case PLAYERSTATE::WALK:       Model::SetAnimFrame(hModel_, 20, 60, 0.5f); break;
        case PLAYERSTATE::RUN:        Model::SetAnimFrame(hModel_, 80, 120, 0.5f); break;
        case PLAYERSTATE::JUMP:       Model::SetAnimFrame(hModel_, 120, 120, 1.0f); break;
        case PLAYERSTATE::STUN:       Model::SetAnimFrame(hModel_, 140, 200, 0.5f); break;
        }
    }
    scoreTimeCounter_++;
    if (scoreTimeCounter_ % FPS_ == scoreTimeCounterWait_)
    {
        score_ += scoreAmount_;
    }
    playerStatePrev_ = playerState_;
    PlayerFall();
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
        playerState_ = PLAYERSTATE::WALK;
        Audio::Play(hSound_[((int)SOUNDSTATE::WALK)], soundVolume_);
    }
    if (!IsMoving() && !isJump_)
    {
        playerState_ = PLAYERSTATE::WAIT;
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER,padID_) && !isJump_ && IsMoving())
    {
        playerState_ = PLAYERSTATE::RUN;
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
        playerState_ = PLAYERSTATE::JUMP;
    }
    if (isStun_)
    {
        playerState_ = PLAYERSTATE::STUN;
    }
}

void AttackPlayer::UpdateGameOver()
{
    Direct3D::SetIsChangeView(((int)Direct3D::VIEWSTATE::RIGHTVIEW));
    if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, collectPlayerNumber))
    {
        pSceneManager_->ChangeScene(SCENE_ID_GAMEOVER);
        PlayerScore_[collectPlayerNumber] = pCollectPlayer_->GetScore();
        PlayerScore_[attackPlayerNumber] = this->GetScore();
    }
}

void AttackPlayer::Stun(int _timeLimit)
{
    isStun_ = true;
    stunLimit_ = _timeLimit;
}

void AttackPlayer::OnCollision(GameObject* _pTarget)
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
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = initZeroInt;
    }

    if (_pTarget->GetObjectName() == collectPlayerName)
    {
        Stun(hitStopTime_);
        isKnockBack_ = true;
        vecKnockbackDirection_ = (XMLoadFloat3(&transform_.position_) - pCollectPlayer_->GetVecPos());
        vecKnockbackDirection_ = XMVector3Normalize(vecKnockbackDirection_);
    }
}

void AttackPlayer::PlayerFall()
{
    if (isJump_)
    {
        //�������ɉ����鏈��
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - gravity_;
        positionPrevY_ = positionTempY_;
        isJump_ = (positionY_ <= -rayFloorDistDown_ + playerInitPosY_) ? false : isJump_;
        isJump_ = (positionY_ <= -rayStageDistDown_ + playerInitPosY_) ? false : isJump_;
    }
}

void AttackPlayer::PlayerMove()
{
    if (!isDash_)
    {
        controllerMoveSpeed_ = walkSpeed_;
    }
    else
    {
        controllerMoveSpeed_ = dashSpeed_;
    }
    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER, padID_)))
    {
        XMVECTOR vecCam = {};
        CamPositionVec_ = Camera::VecGetPosition(attackPlayerNumber);
        vecCam = -(Camera::VecGetPosition(attackPlayerNumber) - Camera::VecGetTarget(attackPlayerNumber));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = initZeroFloat;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        vecMove_ = vecCam;
    }
    //�����ύX
    vecLength_ = XMVector3Length(vecMove_);
    length_ = XMVectorGetX(vecLength_);
    if (length_ != initZeroInt)
    {
        //�v���C���[�����̓L�[�ɉ����āA���̌����ɕς���(�������ɂ͏o���Ȃ�)
        vecMove_ = XMVector3Normalize(vecMove_);

        vecDot_ = XMVector3Dot(vecFront, vecMove_);
        dot_ = XMVectorGetX(vecDot_);
        angle_ = acos(dot_);

        //�E�����ɂ��������Ȃ��������̂��������ɂ��鎖���ł���
        vecCross_ = XMVector3Cross(vecFront, vecMove_);
        if (XMVectorGetY(vecCross_) < initZeroInt)
        {
            angle_ *= -initOneInt;
        }
    }
    transform_.rotate_.y = XMConvertToDegrees(angle_);
    //XMConvertToRadians = degree�p��radian�p��(����)�ϊ�����
    //XMMatrixRotationY = Y���W�𒆐S�ɉ�]������s������֐�
    const XMMATRIX rotmat = XMMatrixRotationY(halfPi_);
    XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(attackPlayerNumber);
    vecDirection = XMVectorSetY(vecDirection, initZeroFloat);
    vecDirection = XMVector3Normalize(vecDirection);
    const float deadZone = 0.3f;			//�R���g���[���[�̃f�b�g�]�[��
    if (Input::GetPadStickL(padID_).y > deadZone)
    {
        transform_.position_.x += controllerMoveSpeed_ * XMVectorGetX(vecDirection);
        transform_.position_.z += controllerMoveSpeed_ * XMVectorGetZ(vecDirection);
    }
    if (Input::GetPadStickL(padID_).y < -deadZone)
    {
        transform_.position_.x += controllerMoveSpeed_ * XMVectorGetX(-vecDirection);
        transform_.position_.z += controllerMoveSpeed_ * XMVectorGetZ(-vecDirection);
    }
    if (Input::GetPadStickL(padID_).x > deadZone)
    {
        XMVECTOR tempvec = XMVector3Transform(vecDirection, rotmat);
        transform_.position_.x += controllerMoveSpeed_ * XMVectorGetX(tempvec);
        transform_.position_.z += controllerMoveSpeed_ * XMVectorGetZ(tempvec);
    }
    if (Input::GetPadStickL(padID_).x < -deadZone)
    {
        XMVECTOR tempvec = XMVector3Transform(vecDirection, -rotmat);
        transform_.position_.x += controllerMoveSpeed_ * XMVectorGetX(tempvec);
        transform_.position_.z += controllerMoveSpeed_ * XMVectorGetZ(tempvec);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_A, padID_) && !isJump_)
    {
        PlayerJumpPower();
        Audio::Stop(hSound_[((int)SOUNDSTATE::WALK)]);
        Audio::Stop(hSound_[((int)SOUNDSTATE::RUN)]);
        Audio::Play(hSound_[((int)SOUNDSTATE::JUMP)],soundVolumeHalf_);
    }
    if (transform_.position_.z <= outerWallPosBack_ || transform_.position_.z >= outerWallPosFront_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    if (transform_.position_.x <= outerWallPosRight_ || transform_.position_.x >= outerWallPosLeft_)
    {
        transform_.position_.x = positionPrev_.x;
    }
}

void AttackPlayer::PlayerJump()
{

}

void AttackPlayer::PlayerJumpPower()
{
    //�W�����v�̏���
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + jumpPower_;
}

void AttackPlayer::PlayerDive()
{
    XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(attackPlayerNumber);
    vecDirection = XMVectorSetY(vecDirection, normalizationInt);
    vecDirection = XMVector3Normalize(vecDirection);
    transform_.position_.x = transform_.position_.x + diveSpeed_ * XMVectorGetX(vecDirection);
    transform_.position_.z = transform_.position_.z + diveSpeed_ * XMVectorGetZ(vecDirection);
    if (diveTime_ >= diveTimeWait_)
    {
        isDive_ = false;
        isDived_ = true;
        diveTime_ = initZeroInt;
    }
}

void AttackPlayer::PlayerDivePower()
{
    //�Ƃт��̏���
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + divePower_;
}

void AttackPlayer::PlayerKnockback()
{
    if (isKnockBack_)
    {
        SetKnockback(vecKnockbackDirection_, knockbackSpeed_);
        Stun(getUpTime_);
    }
}

void AttackPlayer::PlayerRayCast()
{
    RayCastData floorDataUp;
    RayCastData floorDataDown;
    RayCastData stageDataDown;
    RayCastData stageDataFront;
    RayCastData stageDataBack;
    RayCastData stageDataLeft;
    RayCastData stageDataRight;                       //�v���C���[���n�ʂ���ǂ̂��炢����Ă����畂���Ă��锻��ɂ��邩
    stageHModel_ = pStage_->GetModelHandle();         //���f���ԍ����擾
    floorHModel_ = pFloor_->GetModelHandle();

    for (int i = initZeroInt;i < pItemObjectManager_->GetFloors().size(); i++)
    {
        //����̖@��(���蔲�����̂���)
        floorDataUp.start = transform_.position_;           //���C�̔��ˈʒu
        XMStoreFloat3(&floorDataUp.dir, vecUp);             //���C�̕���
        Model::RayCast(floorHModel_ + i, &floorDataUp);         //���C�𔭎�
        rayFloorDistUp_ = floorDataUp.dist;

        //�����̖@��(���蔲����)
        floorDataDown.start = transform_.position_;    //���C�̔��ˈʒu
        floorDataDown.start.y = initZeroFloat;
        XMStoreFloat3(&floorDataDown.dir, vecDown);     //���C�̕���
        if (rayFloorDistUp_ == distMax_)
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
                positionY_ = -rayFloorDistDown_ + playerInitPosY_;
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
            }
        }
        else
        {
            isOnFloor_ = false;
        }

    }
    //�����̖@��(�n�ʂɒ���t��)
    stageDataDown.start = transform_.position_;   //���C�̔��ˈʒu
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataDown); //���C�𔭎�
    rayStageDistDown_ = stageDataDown.dist;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    if (rayStageDistDown_ + positionY_ <= isFling_)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (!isJump_ && !isOnFloor_)
        {
            //�n�ʂɒ���t��
            isDived_ = false;
            positionY_ = -rayStageDistDown_ + playerInitPosY_;
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
    rayStageDistFront_ = stageDataFront.dist;
    if (rayStageDistFront_ <= inTheWall_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    stageDataBack.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataBack.dir, vecBackUp);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataBack);     //���C�𔭎�
    rayStageDistBack_ = stageDataBack.dist;
    if (rayStageDistBack_ <= inTheWall_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    stageDataLeft.start = transform_.position_;       //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataLeft.dir, vecLeftUp);     //���C�̕���
    Model::RayCast(stageHModel_, &stageDataLeft);     //���C�𔭎�
    rayStageDistLeft_ = stageDataLeft.dist;
    if (rayStageDistLeft_ <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    //���E�̖@��(�ǂ̓����蔻��)
    stageDataRight.start = transform_.position_;      //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataRight.dir, vecRightUp);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataRight);    //���C�𔭎�
    rayStageDistRight_ = stageDataRight.dist;
    if (rayStageDistRight_ <= inTheWall_)
    {
        transform_.position_.x = positionPrev_.x;
    }
    positionPrev_ = transform_.position_;
}

void AttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

bool AttackPlayer::IsMoving()
{
    return (transform_.position_.x != positionPrev_.x || transform_.position_.z != positionPrev_.z);
}
