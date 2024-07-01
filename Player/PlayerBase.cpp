//インクルード
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"
#include "../Engine/ImGui/imgui.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(GameObject* _pParent,std::string _name)
    :GameObject(_pParent, _name)
{
    stunData_.vecKnockbackDirection_ = {};
    dirData_.vecCam_ = { XMFLOAT3(0.0f,5.0f,-10.0f) };
    gameData_.walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlayScene.ini");
}

// 初期化
void PlayerBase::Initialize()
{

}

// 更新
void PlayerBase::Update()
{

}

// 描画
void PlayerBase::BothViewDraw()
{

}

void PlayerBase::LeftViewDraw()
{

}

void PlayerBase::RightViewDraw()
{

}

void PlayerBase::UPSubViewDraw()
{
}

// 開放
void PlayerBase::Release()
{

}

void PlayerBase::UpdateReady()
{

}

void PlayerBase::UpdatePlay()
{

}

void PlayerBase::UpdateGameOver()
{

}

void PlayerBase::PlayerWaitStateFunc()
{
    animData_.startFrame_ = 0;
    animData_.endFrame_ = 0;
    animData_.animSpeed_ = 1.0f;
}

void PlayerBase::PlayerWalkStateFunc()
{
    animData_.startFrame_ = 20;
    animData_.endFrame_ = 60;
    animData_.animSpeed_ = 0.5f;
}

void PlayerBase::PlayerRunStateFunc()
{
    animData_.startFrame_ = 80;
    animData_.endFrame_ = 120;
    animData_.animSpeed_ = 0.5f;
}

void PlayerBase::PlayerJumpStateFunc()
{
    animData_.startFrame_ = 120;
    animData_.endFrame_ = 120;
    animData_.animSpeed_ = 1.0f;
}

void PlayerBase::PlayerStunStateFunc()
{
    animData_.startFrame_ = 140;
    animData_.endFrame_ = 200;
    animData_.animSpeed_ = 0.25f;
}

void PlayerBase::PlayerScore()
{
    gameData_.score_ += gameData_.scoreAmount_;
}

void PlayerBase::PlayerCamera()
{

}

void PlayerBase::PlayerFall()
{
    // プレイヤーが落下する処理
    if (jumpData_.isJump_)
    {
        jumpData_.positionTempY_ = jumpData_.positionY_;
        jumpData_.positionY_ += (jumpData_.positionY_ - jumpData_.positionPrevY_) - jumpData_.gravity_;
        jumpData_.positionPrevY_ = jumpData_.positionTempY_;
        IsJump();
    }
}

void PlayerBase::PlayerMove()
{

}

void PlayerBase::PlayerJump()
{
    // プレイヤーのジャンプ処理
}

void PlayerBase::PlayerJumpPower()
{
    // ジャンプ時の力を計算する処理
    jumpData_.isJump_ = true;
    jumpData_.positionPrevY_ = jumpData_.positionY_;
    jumpData_.positionY_ += jumpData_.jumpPower_;
}

void PlayerBase::PlayerDive()
{
    // プレイヤーのダイブ処理
    dirData_.vecDirection_ = XMVectorSetY(dirData_.vecDirection_, normalizationInt);
    dirData_.vecDirection_ = XMVector3Normalize(dirData_.vecDirection_);
    transform_.position_.x += diveData_.diveSpeed_ * XMVectorGetX(dirData_.vecDirection_);
    transform_.position_.z += diveData_.diveSpeed_ * XMVectorGetZ(dirData_.vecDirection_);

    if (diveData_.diveTime_ >= diveData_.diveTimeWait_)
    {
        diveData_.isDive_ = false;
        diveData_.isDived_ = true;
        diveData_.diveTime_ = 0;
    }
}

void PlayerBase::PlayerDivePower()
{
    // ダイブ時の力を計算する処理
    jumpData_.isJump_ = true;
    jumpData_.positionPrevY_ = jumpData_.positionY_;
    jumpData_.positionY_ += diveData_.divePower_;
}

void PlayerBase::PlayerKnockback()
{
    // プレイヤーのノックバック処理
    if (stunData_.isKnockBack_)
    {
        SetKnockback(stunData_.vecKnockbackDirection_, stunData_.knockbackSpeed_);
        PlayerStun(stunData_.getUpTime_);
    }
}

void PlayerBase::PlayerRayCast()
{
    // レイキャスト処理

}

void PlayerBase::PlayerRevival()
{
    transform_.position_ = initZeroXMFLOAT3;
}

void PlayerBase::PlayerStun(int _timeLimit)
{
    // スタンの処理
    stunData_.isStun_ = true;
    stunData_.stunLimit_ = _timeLimit;
}

void PlayerBase::ApplyMovement(float moveX, float moveZ)
{
    transform_.position_.x += moveX * moveData_.padMoveSpeed_.x;
    transform_.position_.z += moveZ * moveData_.padMoveSpeed_.z;
}

void PlayerBase::SetVecPos(XMVECTOR _vecMove)
{
    // ベクトルの位置を設定する処理
}

void PlayerBase::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    // ノックバックの方向と速度を設定する処理
    transform_.position_.x += _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z += _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

XMVECTOR PlayerBase::GetVecPos()
{
    // ベクトルの位置を取得する処理
    XMVECTOR vecPos;
    vecPos = XMLoadFloat3(&transform_.position_);
    return vecPos; // 仮の戻り値
}

void PlayerBase::IsMove()
{
    // プレイヤーが移動中かどうかを判定する処理
    if (transform_.position_.x != moveData_.positionPrev_.x)
    {
        moveData_.isMove_ = true;
    }
    else if (transform_.position_.z != moveData_.positionPrev_.z)
    {
        moveData_.isMove_ = true;
    }
    else
    {
        moveData_.isMove_ = false;
    }
}

void PlayerBase::IsJump()
{
    if (wallData_.rayStageDistDown_ + jumpData_.positionY_ > moveData_.isFling_ && !floorData_.isOnFloor_)
    {
        jumpData_.isJump_ = true;
    }
    if (jumpData_.positionY_ <= -wallData_.rayFloorDistDown_ + jumpData_.playerInitPosY_)
    {
        jumpData_.isJump_ = false;
    }
    if (jumpData_.positionY_ <= -wallData_.rayStageDistDown_ + jumpData_.playerInitPosY_)
    {
        jumpData_.isJump_ = false;
    }
}

void PlayerBase::IsRun()
{
    moveData_.isRun_ = (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, gameData_.padID_) && !jumpData_.isJump_ && moveData_.isMove_);
}

void PlayerBase::IsStun()
{

}

void PlayerBase::IsDive()
{

}
