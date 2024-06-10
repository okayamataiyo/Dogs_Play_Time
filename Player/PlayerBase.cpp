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
    animData_.animSpeed_ = 0.5f;
}

void PlayerBase::PlayerScore()
{
    gameData_.score_ += gameData_.scoreAmount_;
}

void PlayerBase::PlayerCamera()
{
    struct float2
    {
        float x, y;
    };

    struct XMMATRIX2
    {
        XMMATRIX x, y;
    };
    static float2 padRotMove = {};
    float2 sigmaRot = {};
    XMMATRIX2 mat2Rot = {};
    XMMATRIX matRot = {};
    float deadZone = 0.8f;
    XMVECTOR vecDir = {};
    XMFLOAT3 floDir_ = {};
    static float floLen = 0.0f;
    float floCameraLen = 30.0f;
    float floKnockbackLenRecedes = 5.0f;
    XMFLOAT3 mouseMove = Input::GetMouseMove();
    XMFLOAT3 padStickR = {};
    padStickR.x = Input::GetPadStickR(gameData_.padID_).x;
    if (Input::GetPadStickR(gameData_.padID_).y > deadZone)
    {
        if (moveData_.i_ == false)
        {
            moveData_.i_ = true;
            moveData_.CamPosNum_ -= 1;
        }
    }
    else
    {
        moveData_.i_ = false;
    }
    if (Input::GetPadStickR(gameData_.padID_).y < -deadZone)
    {
        if (moveData_.j_ == false)
        {
            moveData_.j_ = true;
            moveData_.CamPosNum_ += 1;
        }
    }
    else
    {
        moveData_.j_ = false;
    }
    if(moveData_.CamPosNum_ < 0)
	{
		moveData_.CamPosNum_ = 0;
	}
    if (moveData_.CamPosNum_ > 4)
    {
        moveData_.CamPosNum_ = 4;
    }
    dirData_.vecCam_.x = moveData_.CamPos_[moveData_.CamPosNum_];
    const float padSens = 25;
    const float floLenRecedes = 1.0f;
    const float floLenApproach = 1.0f;
    const float degreesMin = 0.0f;
    const float degreesMax = -88.0f;
    const float degreesToRadians = 3.14f / 180.0f;
    padRotMove.x = padStickR.x;
    padRotMove.y = -padStickR.y;

    if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP, gameData_.padID_))
    {
        floLen -= floLenRecedes;
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN, gameData_.padID_))
    {
        floLen += floLenApproach;
    }

    vecDir = vecFront;
    dirData_.vecCam_.x += padRotMove.y / padSens;
    dirData_.vecCam_.y += padRotMove.x / padSens;

    sigmaRot.y = dirData_.vecCam_.y;
    sigmaRot.x = -dirData_.vecCam_.x;

    if (sigmaRot.x > degreesMin * degreesToRadians)
    {
        sigmaRot.x = degreesMin;
        dirData_.vecCam_.x -= padRotMove.y / padSens;
    }
    if (sigmaRot.x < degreesMax * degreesToRadians)
    {
        sigmaRot.x = degreesMax * degreesToRadians;
        dirData_.vecCam_.x -= padRotMove.y / padSens;
    }

    mat2Rot.x = XMMatrixRotationX(sigmaRot.x);
    mat2Rot.y = XMMatrixRotationY(sigmaRot.y);

    matRot = mat2Rot.x * mat2Rot.y;
    vecDir = XMVector3Transform(vecDir, matRot);
    vecDir = XMVector3Normalize(vecDir);
    if (stunData_.isStun_)
    {
        static int floCameraLenPrev = floCameraLen;
        if (floCameraLen <= floCameraLenPrev + floKnockbackLenRecedes)
        {
            ++floCameraLen;
        }
    }
    else
    {
        static int floCameraLenPrev = floCameraLen;
        if (floCameraLen >= floCameraLenPrev - floKnockbackLenRecedes)
        {
            --floCameraLen;
        }
    }
    vecDir *= floLen + floCameraLen;
    vecDir += XMLoadFloat3(&transform_.position_);
    XMStoreFloat3(&floDir_, vecDir);
    Camera::SetPosition(floDir_, gameData_.padID_);
    Camera::SetTarget(transform_.position_, gameData_.padID_);
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
    const float walkSpeed = 0.4f;
    const float runSpeed = 0.6f;
    // プレイヤーの移動処理
    if (!moveData_.isRun_)
    {
        moveData_.padMoveSpeed_ = XMFLOAT3(walkSpeed ,0.0f,walkSpeed);
    }
    else
    {
        moveData_.padMoveSpeed_ = XMFLOAT3(runSpeed, 0.0f, runSpeed);
    }
    //向き変更
    XMFLOAT3 m;
    XMStoreFloat3(&m, dirData_.vecMove_);
    transform_.rotate_.y = XMConvertToDegrees(atan2(m.x, m.z));
    dirData_.angle_ = XMConvertToDegrees(atan2(m.x, m.z));

    float pi = 3.14f;					//円周率
    float halfPi = pi / 2;				//円周率の半分

    //XMConvertToRadians = degree角をradian角に(ただ)変換する
    //XMMatrixRotationY = Y座標を中心に回転させる行列を作る関数
    const XMMATRIX rotmat = XMMatrixRotationY(halfPi);
    dirData_.vecDirection_ = XMVectorSetY(dirData_.vecDirection_, 0);
    dirData_.vecDirection_ = XMVector3Normalize(dirData_.vecDirection_);

    const float deadZone = 0.3f;		//コントローラーのデットゾーン
    const float plusDir = 1.0f;
    const float minusDir = -1.0f;
    moveData_.padMoveSpeed_.x *= XMVectorGetX(dirData_.vecDirection_);
    moveData_.padMoveSpeed_.z *= XMVectorGetZ(dirData_.vecDirection_);
    XMVECTOR tempvec = XMVector3Transform(dirData_.vecDirection_, rotmat);
    if (Input::GetPadStickL(gameData_.padID_).y > deadZone)   //前への移動
    {
        ApplyMovement(plusDir, plusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).y < -deadZone)  //後ろへの移動
    {
        ApplyMovement(minusDir, minusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).x > deadZone)   //右への移動
    {
        moveData_.padMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        moveData_.padMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        ApplyMovement(plusDir, plusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).x < -deadZone)  //左への移動
    {
        moveData_.padMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        moveData_.padMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        ApplyMovement(minusDir, minusDir);
    }

    const float outerWallPosFront = 99.0f;		//前の外壁の位置
    const float outerWallPosBack = -99.0f;		//後ろの外壁の位置
    const float outerWallPosLeft = 99.0f;		//左の外壁の位置
    const float outerWallPosRight = -99.0f;		//右の外壁の位置

    if (transform_.position_.z <= outerWallPosBack || transform_.position_.z >= outerWallPosFront)
    {
        transform_.position_.z = moveData_.positionPrev_.z;
    }
    if (transform_.position_.x <= outerWallPosRight || transform_.position_.x >= outerWallPosLeft)
    {
        transform_.position_.x = moveData_.positionPrev_.x;
    }
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
    if (Input::GetPadTrrigerR(gameData_.padID_))
    {
        diveData_.isDive_ = true;
    }
}
