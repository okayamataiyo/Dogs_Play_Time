//�C���N���[�h
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(GameObject* _pParent,std::string _name)
    :GameObject(_pParent, _name),vecKnockbackDirection_ {}
{

}

// ������
void PlayerBase::Initialize()
{

}

// �X�V
void PlayerBase::Update()
{

    PlayerMove();
    PlayerJump();
    PlayerDive();
    PlayerKnockback();
}

// �`��
void PlayerBase::Draw()
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

void PlayerBase::PlayerFall()
{
    // �v���C���[���������鏈��
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
    // �v���C���[�̈ړ�����
    if (!isDash_)
    {
        controllerMoveSpeed_ = XMFLOAT3(walkSpeed ,0.0f,walkSpeed);
    }
    else
    {
        controllerMoveSpeed_ = XMFLOAT3(dashSpeed, 0.0f, dashSpeed);
    }
    //�����ύX
    XMVECTOR vecLength = XMVector3Length(vecMove_);
    float length = XMVectorGetX(vecLength);

    const float angleInversion = -1.0f;

    if (length != 0.0f)
    {
        //�v���C���[�����̓L�[�ɉ����āA���̌����ɕς���(�������ɂ͏o���Ȃ�)
        vecMove_ = XMVector3Normalize(vecMove_);

        XMVECTOR vecDot = XMVector3Dot(vecFront, vecMove_);
        float dot = XMVectorGetX(vecDot);
        angle_ = acos(dot);

        //�E�����ɂ��������Ȃ��������̂��������ɂ��鎖���ł���
        vecCross_ = XMVector3Cross(vecFront, vecMove_);
        if (XMVectorGetY(vecCross_) < 0.0f)
        {
            angle_ *= angleInversion;
        }
    }

    transform_.rotate_.y = XMConvertToDegrees(angle_);

    float pi = 3.14f;					//�~����
    float halfPi = pi / 2;				//�~�����̔���

    //XMConvertToRadians = degree�p��radian�p��(����)�ϊ�����
    //XMMatrixRotationY = Y���W�𒆐S�ɉ�]������s������֐�
    const XMMATRIX rotmat = XMMatrixRotationY(halfPi);
    vecDirection_ = XMVectorSetY(vecDirection_, 0);
    vecDirection_ = XMVector3Normalize(vecDirection_);

    const float deadZone = 0.3f;			//�R���g���[���[�̃f�b�g�]�[��
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

    const float outerWallPosFront = 99.0f;		//�O�̊O�ǂ̈ʒu
    const float outerWallPosBack = -99.0f;		//���̊O�ǂ̈ʒu
    const float outerWallPosLeft = 99.0f;		//���̊O�ǂ̈ʒu
    const float outerWallPosRight = -99.0f;		//�E�̊O�ǂ̈ʒu

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
    // �v���C���[�̃W�����v����
}

void PlayerBase::PlayerJumpPower()
{
    // �W�����v���̗͂��v�Z���鏈��
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + jumpPower_;
}

void PlayerBase::PlayerDive()
{
    // �v���C���[�̃_�C�u����
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
    // �_�C�u���̗͂��v�Z���鏈��
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + divePower_;
}

void PlayerBase::PlayerKnockback()
{
    // �v���C���[�̃m�b�N�o�b�N����
    if (isKnockBack_)
    {
        SetKnockback(vecKnockbackDirection_, knockbackSpeed_);
        Stun(getUpTime_);
    }
}

void PlayerBase::PlayerRayCast()
{
    // ���C�L���X�g����

}

void PlayerBase::Stun(int _timeLimit)
{
    // �X�^���̏���
    isStun_ = true;
    stunLimit_ = _timeLimit;
}

void PlayerBase::SetVecPos(XMVECTOR _vecMove)
{
    // �x�N�g���̈ʒu��ݒ肷�鏈��
}

void PlayerBase::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    // �m�b�N�o�b�N�̕����Ƒ��x��ݒ肷�鏈��
}

XMVECTOR PlayerBase::GetVecPos()
{
    // �x�N�g���̈ʒu���擾���鏈��
    return XMVECTOR(); // ���̖߂�l
}

bool PlayerBase::IsMoving()
{
    // �v���C���[���ړ������ǂ����𔻒肷�鏈��
    return false; // ���̖߂�l
}