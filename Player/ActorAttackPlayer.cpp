//インクルード
#include "../Engine/Global.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/ImGui/imgui.h"
#include "../Scene/PlayScene.h"
#include "../StageObject/Stage.h"
#include "ActorAttackPlayer.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"

ActorAttackPlayer::ActorAttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, actorAttackPlayerName), hModel_{ -1 }, stageHModel_{ -1 }, playerState_{ PLAYERSTATE::WAIT }, playerStatePrev_{ PLAYERSTATE::WAIT }, gameState_{ GAMESTATE::READY }
    ,pPlayScene_{nullptr},pStage_{nullptr}
{
    //▼UIに関する基底クラスメンバ変数
    drawScoreTextX_ = 30;
    drawScoreTextY_ = 30;
    drawScoreNumberX_ = 360;
    drawScoreNumberY_ = 30;
    //▼ゲーム演出に関する基底クラスメンバ変数
    FPS_ = 60 * 10;
    timeCounter_ = 0;
    timeLimit_ = 60;
    fallLimit_ = 100.0f;
    score_ = 0;
    scoreAmount_ = 10;
    scoreMax_ = 150;
    padID_ = 0;
    playerInitPosY_ = 0.6f;
    //▼サウンドに関する基底クラスメンバ変数
    soundVolume_ = 0.5f;
    soundVolumeHalf_ = soundVolume_ / 2;
    //▼邪魔側プレイヤー移動に関する基底クラスメンバ変数
    CamPositionVec_ = {};
    positionPrev_ = { 0.0f,0.0f,0.0f };
    controllerMoveSpeed_ = 0.3f;
    mouseMoveSpeed_ = 0.3f;
    positionY_ = 0.0f;
    isDash_ = false;
    isFling_ = 1.0f;
    deadZone_ = 0.3f;
    pi_ = 3.14;
    halfPi_ = pi_ / 2.0f;
    dashSpeed_ = 0.5f;
    walkSpeed_ = 0.4f;
    //▼向き変えに関する基底クラスメンバ変数
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecLength_ = { 0.0f,0.0f,0.0f,0.0f };
    vecDot_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    length_ = 0.0f;
    dot_ = 0.0f;
    angle_ = 0.0f;
    //▼邪魔側プレイヤージャンプに関する基底クラスメンバ変数
    gravity_ = 0.007f;
    positionTempY_ = 0.0f;
    positionPrevY_ = 0.0f;
    jumpPower_ = 0.3f;
    isJump_ = false;
    //▼飛びつきに関する基底クラスメンバ変数
    divePower_ = 0.1f;
    diveSpeed_ = 0.2f;
    isDive_ = false;
    isDived_ = false;
    diveTime_ = 0;
    diveDuration_ = 1;
    diveTimeWait_ = 30;
    //▼すり抜け床に関する基底クラスメンバ変数
    isOnFloor_ = false;
    //▼木箱に関する基底クラスメンバ変数
    woodBoxNumber_ = "";
    dotProduct_ = 0.0f;
    angleDegrees_ = 0.0f;
    angleDegreesMax_ = 80.0f;
    //▼スタンに関する基底クラスメンバ変数
    stunTimeCounter_ = 0;
    stunLimit_ = 0;
    hitStopTime_ = 10;
    getUpTime_ = 30;
    knockbackSpeed_ = 0.3f;
    isStun_ = false;
    isKnockBack_ = false;
    //▼壁判定に関する基底クラスメンバ変数
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

ActorAttackPlayer::~ActorAttackPlayer()
{
}

void ActorAttackPlayer::Initialize()
{
    //▼モデルデータのロード
    std::string modelName = modelFolderName + actorAttackPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    transform_.scale_ = { 0.4f,0.4f,0.4f };
    positionY_ = transform_.position_.y;
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pStage_ = (Stage*)FindObject(stageName);
}

void ActorAttackPlayer::Update()
{
    //落ちた時の処理
    if (transform_.position_.y <= -fallLimit_)
    {
        transform_.position_ = initZeroXMFLOAT3;
    }

    isDive_ = true;

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
    playerStatePrev_ = playerState_;
    PlayerFall();
    PlayerMove();
    PlayerRayCast();
    transform_.position_.y = positionY_;
    if (IsMoving() && !isJump_ && !isDash_)
    {
        playerState_ = PLAYERSTATE::WALK;
    }
    if (!IsMoving() && !isJump_)
    {
        playerState_ = PLAYERSTATE::WAIT;
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, padID_) && !isJump_ && IsMoving())
    {
        playerState_ = PLAYERSTATE::RUN;
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

void ActorAttackPlayer::Stun(int _timeLimit)
{
}



void ActorAttackPlayer::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void ActorAttackPlayer::Release()
{
}

void ActorAttackPlayer::UpdateReady()
{
}

void ActorAttackPlayer::UpdatePlay()
{
}

void ActorAttackPlayer::UpdateGameOver()
{
}

void ActorAttackPlayer::OnCollision(GameObject* _pTarget)
{
}

void ActorAttackPlayer::PlayerFall()
{
    if (isJump_)
    {
        //放物線に下がる処理
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - gravity_;
        positionPrevY_ = positionTempY_;
        isJump_ = (positionY_ <= -rayStageDistDown_ + playerInitPosY_) ? false : isJump_;
    }
}

void ActorAttackPlayer::PlayerMove()
{
    if (!isDash_)
    {
        controllerMoveSpeed_ = walkSpeed_;
    }
    else
    {
        controllerMoveSpeed_ = dashSpeed_;
    }
    //向き変更
    vecLength_ = XMVector3Length(vecMove_);
    length_ = XMVectorGetX(vecLength_);
    if (length_ != initZeroInt)
    {
        //プレイヤーが入力キーに応じて、その向きに変える(左向きには出来ない)
        vecMove_ = XMVector3Normalize(vecMove_);

        vecDot_ = XMVector3Dot(vecFront, vecMove_);
        dot_ = XMVectorGetX(vecDot_);
        angle_ = acos(dot_);

        //右向きにしか向けなかったものを左向きにする事ができる
        vecCross_ = XMVector3Cross(vecFront, vecMove_);
        if (XMVectorGetY(vecCross_) < initZeroInt)
        {
            angle_ *= -initOneInt;
        }
    }
    transform_.rotate_.y = XMConvertToDegrees(angle_);
    if (transform_.position_.z <= outerWallPosBack_ || transform_.position_.z >= outerWallPosFront_)
    {
        transform_.position_.z = positionPrev_.z;
    }
    if (transform_.position_.x <= outerWallPosRight_ || transform_.position_.x >= outerWallPosLeft_)
    {
        transform_.position_.x = positionPrev_.x;
    }

    XMMATRIX rotmat = XMMatrixRotationY(halfPi_);
    XMVECTOR vecDirection = vecBackLeft;//XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(attackPlayerNumber);
    vecDirection = XMVectorSetY(vecDirection, initZeroFloat);
    vecDirection = XMVector3Normalize(vecDirection);
    XMVECTOR tempvec = XMVector3Transform(vecDirection, rotmat);
    transform_.position_.x = transform_.position_.x + controllerMoveSpeed_ * XMVectorGetX(tempvec);
    transform_.position_.z = transform_.position_.z + controllerMoveSpeed_ * XMVectorGetZ(tempvec);
}

void ActorAttackPlayer::PlayerJump()
{
}

void ActorAttackPlayer::PlayerJumpPower()
{
}

void ActorAttackPlayer::PlayerDive()
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

void ActorAttackPlayer::PlayerDivePower()
{
    //とびつきの処理
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + divePower_;
}

void ActorAttackPlayer::PlayerKnockback()
{
}

void ActorAttackPlayer::PlayerRayCast()
{
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    //▼下の法線(地面に張り付き)
    stageDataDown.start = transform_.position_;   //レイの発射位置
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);   //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown); //レイを発射
    rayStageDistDown_ = stageDataDown.dist;
    //プレイヤーが浮いていないとき
    if (rayStageDistDown_ + positionY_ <= isFling_)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (!isJump_ && !isOnFloor_)
        {
            //地面に張り付き
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
}

void ActorAttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
}

bool ActorAttackPlayer::IsMoving()
{
    return (transform_.position_.x != positionPrev_.x || transform_.position_.z != positionPrev_.z);
}
