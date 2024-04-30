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
    :PlayerBase(_pParent, actorAttackPlayerName), hModel_{ -1 }, stageHModel_{ -1 }, gameState_{ GAMESTATE::READY }
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
    controllerMoveSpeed_ = {0.3f,0.0f,0.3f};
    positionY_ = 0.0f;
    isRun_ = false;
    isFling_ = 1.0f;
    //▼向き変えに関する基底クラスメンバ変数
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    vecDirection_ = { 0.0f,0.0f,0.0f,0.0f };
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
    rayFloorDistDown_ = 0.0f;
    rayStageDistDown_ = 0.0f;
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

    PlayerFall();
    PlayerMove();
    PlayerRayCast();
    transform_.position_.y = positionY_;
    if (isMove_ && !isJump_ && !isRun_)
    {
    }
    if (!isMove_ && !isJump_)
    {
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, padID_) && !isJump_ && isMove_)
    {
        isRun_ = true;
    }
    else
    {
        isRun_ = false;
    }
    if (isJump_)
    {
    }
    if (isStun_)
    {
    }
}

void ActorAttackPlayer::PlayerStun(int _timeLimit)
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
    PlayerBase::PlayerMove();

    transform_.position_.x += controllerMoveSpeed_.x;
    transform_.position_.z += controllerMoveSpeed_.z;
}

void ActorAttackPlayer::IsMove()
{
    PlayerBase::IsMove();
}

void ActorAttackPlayer::IsJump()
{
    PlayerBase::IsJump();
}

void ActorAttackPlayer::IsRun()
{
    PlayerBase::IsRun();
}

void ActorAttackPlayer::IsStun()
{
    PlayerBase::IsStun();
}

void ActorAttackPlayer::IsDive()
{
    PlayerBase::IsDive();
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