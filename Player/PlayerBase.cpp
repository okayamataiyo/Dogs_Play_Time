//�C���N���[�h
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(GameObject* _pParent,std::string _name)
    :GameObject(_pParent, _name)
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

// ���̑��̃��\�b�h�̎������K�v�ł�
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
    // �v���C���[�̈ړ�����
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
    // �v���C���[�̃W�����v����
}

void PlayerBase::PlayerJumpPower()
{
    // �W�����v���̗͂��v�Z���鏈��
}

void PlayerBase::PlayerDive()
{
    // �v���C���[�̃_�C�u����
}

void PlayerBase::PlayerDivePower()
{
    // �_�C�u���̗͂��v�Z���鏈��
}

void PlayerBase::PlayerKnockback()
{
    // �v���C���[�̃m�b�N�o�b�N����
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