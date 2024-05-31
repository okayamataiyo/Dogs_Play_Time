//インクルード
#include "../Engine/Global.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/ImGui/imgui.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "../StageObject/Stage.h"
#include "ActorAttackPlayer.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"

ActorAttackPlayer::ActorAttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, actorAttackPlayerName), hModel_{ -1 }, stageHModel_{ -1 },isSelect_{false}, gameState_{GAMESTATE::READY}
    ,pDogs_Walk_PlayScene_{nullptr},pStage_{nullptr}
{
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
    jumpData_.positionY_ = transform_.position_.y;
    pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
    pStage_ = (Stage*)FindObject(stageName);
}

void ActorAttackPlayer::Update()
{
    UpdatePlay();
    
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
    //落ちた時の処理
    if (transform_.position_.y <= -gameData_.fallLimit_)
    {
        transform_.position_ = initZeroXMFLOAT3;
    }

    if (!isSelect_)
    {
        //diveData_.isDive_ = true;
        //jumpData_.isJump_ = true;
        PlayerJumpPower();
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

    PlayerFall();
    PlayerMove();
    PlayerKnockback();
    if (!isSelect_)
    {
        PlayerRayCast();
    }
    transform_.position_.y = jumpData_.positionY_;
    IsMove();
    IsJump();
    IsRun();
    IsStun();
    IsDive();
}

void ActorAttackPlayer::UpdateGameOver()
{
}

void ActorAttackPlayer::OnCollision(GameObject* _pTarget)
{
}

void ActorAttackPlayer::PlayerScore()
{
}

void ActorAttackPlayer::PlayerCamera()
{
}

void ActorAttackPlayer::PlayerFall()
{
    if (jumpData_.isJump_)
    {
        //放物線に下がる処理
        jumpData_.positionTempY_ = jumpData_.positionY_;
        jumpData_.positionY_ += (jumpData_.positionY_ - jumpData_.positionPrevY_) - jumpData_.gravity_;
        jumpData_.positionPrevY_ = jumpData_.positionTempY_;
    }
}

void ActorAttackPlayer::PlayerMove()
{
    PlayerBase::PlayerMove();

    transform_.position_.x += moveData_.padMoveSpeed_.x;
    transform_.position_.z += moveData_.padMoveSpeed_.z;
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
    XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition((int)PADIDSTATE::SECONDS);
    vecDirection = XMVectorSetY(vecDirection, normalizationInt);
    vecDirection = XMVector3Normalize(vecDirection);
    transform_.position_.x = transform_.position_.x + diveData_.diveSpeed_ * XMVectorGetX(vecDirection);
    transform_.position_.z = transform_.position_.z + diveData_.diveSpeed_ * XMVectorGetZ(vecDirection);
    if (diveData_.diveTime_ >= diveData_.diveTimeWait_)
    {
        diveData_.isDive_ = false;
        diveData_.isDived_ = true;
        diveData_.diveTime_ = initZeroInt;
    }
}

void ActorAttackPlayer::PlayerDivePower()
{
    //とびつきの処理
    jumpData_.isJump_ = true;
    jumpData_.positionPrevY_ = jumpData_.positionY_;
    jumpData_.positionY_ += diveData_.divePower_;
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
    wallData_.rayStageDistDown_ = stageDataDown.dist;
    //プレイヤーが浮いていないとき
    if (wallData_.rayStageDistDown_ + jumpData_.positionY_ <= moveData_.isFling_)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (!jumpData_.isJump_ && !floorData_.isOnFloor_)
        {
            //地面に張り付き
            diveData_.isDived_ = false;
            jumpData_.positionY_ = -wallData_.rayStageDistDown_ + jumpData_.playerInitPosY_;
            jumpData_.positionTempY_ = jumpData_.positionY_;
            jumpData_.positionPrevY_ = jumpData_.positionTempY_;
        }
    }
    else if (!floorData_.isOnFloor_)
    {
        jumpData_.isJump_ = true;
    }
    moveData_.positionPrev_ = transform_.position_;
}

void ActorAttackPlayer::PlayerRevival()
{
}

void ActorAttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
}