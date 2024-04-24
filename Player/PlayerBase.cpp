//インクルード
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(GameObject* _pParent,std::string _name)
    :GameObject(_pParent, _name),vecKnockbackDirection_ {}
{

}

// 初期化
void PlayerBase::Initialize()
{

}

// 更新
void PlayerBase::Update()
{

    PlayerMove();
    PlayerJump();
    PlayerDive();
    PlayerKnockback();
}

// 描画
void PlayerBase::Draw()
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

void PlayerBase::PlayerFall()
{
    // プレイヤーが落下する処理
    if (isJump_)
    {
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - gravity_;
        positionPrevY_ = positionTempY_;
        isJump_ = (positionY_ <= -rayFloorDistDown_ + playerInitPosY_) ? false : isJump_;
        isJump_ = (positionY_ <= -rayStageDistDown_ + playerInitPosY_) ? false : isJump_;
    }
}

void PlayerBase::PlayerMove()
{
    const float walkSpeed = 0.4f;
    const float dashSpeed = 0.5f;
    // プレイヤーの移動処理
    if (!isDash_)
    {
        controllerMoveSpeed_ = XMFLOAT3(walkSpeed ,0.0f,walkSpeed);
    }
    else
    {
        controllerMoveSpeed_ = XMFLOAT3(dashSpeed, 0.0f, dashSpeed);
    }
    //向き変更
    XMVECTOR vecLength = XMVector3Length(vecMove_);
    float length = XMVectorGetX(vecLength);

    const float angleInversion = -1.0f;

    if (length != 0.0f)
    {
        //プレイヤーが入力キーに応じて、その向きに変える(左向きには出来ない)
        vecMove_ = XMVector3Normalize(vecMove_);

        XMVECTOR vecDot = XMVector3Dot(vecFront, vecMove_);
        float dot = XMVectorGetX(vecDot);
        angle_ = acos(dot);

        //右向きにしか向けなかったものを左向きにする事ができる
        vecCross_ = XMVector3Cross(vecFront, vecMove_);
        if (XMVectorGetY(vecCross_) < 0.0f)
        {
            angle_ *= angleInversion;
        }
    }

    transform_.rotate_.y = XMConvertToDegrees(angle_);

    float pi = 3.14f;					//円周率
    float halfPi = pi / 2;				//円周率の半分

    //XMConvertToRadians = degree角をradian角に(ただ)変換する
    //XMMatrixRotationY = Y座標を中心に回転させる行列を作る関数
    const XMMATRIX rotmat = XMMatrixRotationY(halfPi);
    vecDirection_ = XMVectorSetY(vecDirection_, 0);
    vecDirection_ = XMVector3Normalize(vecDirection_);

    const float deadZone = 0.3f;			//コントローラーのデットゾーン
    controllerMoveSpeed_.x *= XMVectorGetX(vecDirection_);
    controllerMoveSpeed_.z *= XMVectorGetZ(vecDirection_);
    XMVECTOR tempvec = XMVector3Transform(vecDirection_, rotmat);
    if (Input::GetPadStickL(padID_).y > deadZone)
    {
        transform_.position_.x += controllerMoveSpeed_.x;
        transform_.position_.z += controllerMoveSpeed_.z;
    }
    if (Input::GetPadStickL(padID_).y < -deadZone)
    {
        transform_.position_.x -= controllerMoveSpeed_.x;
        transform_.position_.z -= controllerMoveSpeed_.z;
    }
    if (Input::GetPadStickL(padID_).x > deadZone)
    {
        controllerMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        controllerMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        transform_.position_.x += controllerMoveSpeed_.x;
        transform_.position_.z += controllerMoveSpeed_.z;
    }
    if (Input::GetPadStickL(padID_).x < -deadZone)
    {
        controllerMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        controllerMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        transform_.position_.x -= controllerMoveSpeed_.x;
        transform_.position_.z -= controllerMoveSpeed_.z;
    }

    const float outerWallPosFront = 99.0f;		//前の外壁の位置
    const float outerWallPosBack = -99.0f;		//後ろの外壁の位置
    const float outerWallPosLeft = 99.0f;		//左の外壁の位置
    const float outerWallPosRight = -99.0f;		//右の外壁の位置

    if (transform_.position_.z <= outerWallPosBack || transform_.position_.z >= outerWallPosFront)
    {
        transform_.position_.z = positionPrev_.z;
    }
    if (transform_.position_.x <= outerWallPosRight || transform_.position_.x >= outerWallPosLeft)
    {
        transform_.position_.x = positionPrev_.x;
    }
}

void PlayerBase::PlayerJump()
{
    // プレイヤーのジャンプ処理
}

void PlayerBase::PlayerJumpPower()
{
    // ジャンプ時の力を計算する処理
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + jumpPower_;
}

void PlayerBase::PlayerDive()
{
    // プレイヤーのダイブ処理
    vecDirection_ = XMVectorSetY(vecDirection_, normalizationInt);
    vecDirection_ = XMVector3Normalize(vecDirection_);
    transform_.position_.x += diveSpeed_ * XMVectorGetX(vecDirection_);
    transform_.position_.z += diveSpeed_ * XMVectorGetZ(vecDirection_);

    if (diveTime_ >= diveTimeWait_)
    {
        isDive_ = false;
        isDived_ = true;
        diveTime_ = 0;
    }
}

void PlayerBase::PlayerDivePower()
{
    // ダイブ時の力を計算する処理
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + divePower_;
}

void PlayerBase::PlayerKnockback()
{
    // プレイヤーのノックバック処理
    if (isKnockBack_)
    {
        SetKnockback(vecKnockbackDirection_, knockbackSpeed_);
        Stun(getUpTime_);
    }
}

void PlayerBase::PlayerRayCast()
{
    // レイキャスト処理

}

void PlayerBase::Stun(int _timeLimit)
{
    // スタンの処理
    isStun_ = true;
    stunLimit_ = _timeLimit;
}

void PlayerBase::SetVecPos(XMVECTOR _vecMove)
{
    // ベクトルの位置を設定する処理
}

void PlayerBase::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    // ノックバックの方向と速度を設定する処理
}

XMVECTOR PlayerBase::GetVecPos()
{
    // ベクトルの位置を取得する処理
    return XMVECTOR(); // 仮の戻り値
}

bool PlayerBase::IsMoving()
{
    // プレイヤーが移動中かどうかを判定する処理
    return false; // 仮の戻り値
}