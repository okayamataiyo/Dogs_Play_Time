//�C���N���[�h
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

// ������
void PlayerBase::Initialize()
{

}

// �X�V
void PlayerBase::Update()
{

}

// �`��
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

// �J��
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
    // �v���C���[���������鏈��
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
    // �v���C���[�̃W�����v����
}

void PlayerBase::PlayerJumpPower()
{
    // �W�����v���̗͂��v�Z���鏈��
    jumpData_.isJump_ = true;
    jumpData_.positionPrevY_ = jumpData_.positionY_;
    jumpData_.positionY_ += jumpData_.jumpPower_;
}

void PlayerBase::PlayerDive()
{
    // �v���C���[�̃_�C�u����
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
    // �_�C�u���̗͂��v�Z���鏈��
    jumpData_.isJump_ = true;
    jumpData_.positionPrevY_ = jumpData_.positionY_;
    jumpData_.positionY_ += diveData_.divePower_;
}

void PlayerBase::PlayerKnockback()
{
    // �v���C���[�̃m�b�N�o�b�N����
    if (stunData_.isKnockBack_)
    {
        SetKnockback(stunData_.vecKnockbackDirection_, stunData_.knockbackSpeed_);
        PlayerStun(stunData_.getUpTime_);
    }
}

void PlayerBase::PlayerRayCast()
{
    // ���C�L���X�g����

}

void PlayerBase::PlayerRevival()
{
    transform_.position_ = initZeroXMFLOAT3;
}

void PlayerBase::PlayerStun(int _timeLimit)
{
    // �X�^���̏���
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
    // �x�N�g���̈ʒu��ݒ肷�鏈��
}

void PlayerBase::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    // �m�b�N�o�b�N�̕����Ƒ��x��ݒ肷�鏈��
    transform_.position_.x += _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z += _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

XMVECTOR PlayerBase::GetVecPos()
{
    // �x�N�g���̈ʒu���擾���鏈��
    XMVECTOR vecPos;
    vecPos = XMLoadFloat3(&transform_.position_);
    return vecPos; // ���̖߂�l
}

void PlayerBase::IsMove()
{
    // �v���C���[���ړ������ǂ����𔻒肷�鏈��
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
