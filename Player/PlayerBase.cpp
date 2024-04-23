//インクルード
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(GameObject* _pParent,std::string _name)
    :GameObject(_pParent, _name)
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

// その他のメソッドの実装も必要です
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
    // プレイヤーの移動処理
    if (!isDash_)
    {
        controllerMoveSpeed_ = walkSpeed_;
    }
    else
    {
        controllerMoveSpeed_ = dashSpeed_;
    }
}

void PlayerBase::PlayerJump()
{
    // プレイヤーのジャンプ処理
}

void PlayerBase::PlayerJumpPower()
{
    // ジャンプ時の力を計算する処理
}

void PlayerBase::PlayerDive()
{
    // プレイヤーのダイブ処理
}

void PlayerBase::PlayerDivePower()
{
    // ダイブ時の力を計算する処理
}

void PlayerBase::PlayerKnockback()
{
    // プレイヤーのノックバック処理
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