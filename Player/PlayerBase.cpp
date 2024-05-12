//�C���N���[�h
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Global.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(GameObject* _pParent,std::string _name)
    :GameObject(_pParent, _name),vecKnockbackDirection_ {},floDir_{},floCameraLen_{30.0f}
    ,floKnockbackLenRecedes_{5.0f}
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

void PlayerBase::PlayerWaitStateFunc()
{
    startFrame_ = 0;
    endFrame_ = 0;
    animSpeed_ = 1.0f;
}

void PlayerBase::PlayerWalkStateFunc()
{
    startFrame_ = 20;
    endFrame_ = 60;
    animSpeed_ = 0.5f;
}

void PlayerBase::PlayerRunStateFunc()
{
    startFrame_ = 80;
    endFrame_ = 120;
    animSpeed_ = 0.5f;
}

void PlayerBase::PlayerJumpStateFunc()
{
    startFrame_ = 120;
    endFrame_ = 120;
    animSpeed_ = 1.0f;
}

void PlayerBase::PlayerStunStateFunc()
{
    startFrame_ = 140;
    endFrame_ = 200;
    animSpeed_ = 0.5f;
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

    float2 padRot = {};
    float2 sigmaRot = {};
    XMMATRIX2 mat2Rot = {};
    XMMATRIX matRot = {};

    XMVECTOR vecDir = {};
    float floLen = 0.0f;
    XMFLOAT3 mouseMove = Input::GetMouseMove();
    XMFLOAT3 padStickR = Input::GetPadStickR(padID_);
    const float padSens = 50;
    const float floLenRecedes = 1.0f;
    const float floLenApproach = 1.0f;
    XMFLOAT3 vecCam = XMFLOAT3(0, 5, -10);
    const float degreesMin = 0.0f;
    const float degreesMax = -88.0f;
    const float degreesToRadians = 3.14f / 180.0f;
    padRot.x = padStickR.x;
    padRot.y = -padStickR.y;

    if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP, padID_))
    {
        floLen -= floLenRecedes;
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN, padID_))
    {
        floLen += floLenApproach;
    }

    vecDir = vecFront;
    vecCam.x += padRot.y / padSens;
    vecCam.y += padRot.x / padSens;

    sigmaRot.y = vecCam.y;
    sigmaRot.x = -vecCam.x;

    if (sigmaRot.x > degreesMin * degreesToRadians)
    {
        sigmaRot.x = degreesMin;
        vecCam.x -= padRot.y / padSens;
    }
    if (sigmaRot.x < degreesMax * degreesToRadians)
    {
        sigmaRot.x = degreesMax * degreesToRadians;
        vecCam.x -= padRot.y / padSens;
    }

    mat2Rot.x = XMMatrixRotationX(sigmaRot.x);
    mat2Rot.y = XMMatrixRotationY(sigmaRot.y);

    matRot = mat2Rot.x * mat2Rot.y;
    vecDir = XMVector3Transform(vecDir, matRot);
    vecDir = XMVector3Normalize(vecDir);
    if (isStun_)
    {
        static float floCameraLenPrev = floCameraLen_;
        if (floCameraLen_ <= floCameraLenPrev + floKnockbackLenRecedes_)
        {
            ++floCameraLen_;
        }
    }
    else
    {
        static float floCameraLenPrev = floCameraLen_;
        if (floCameraLen_ >= floCameraLenPrev - floKnockbackLenRecedes_)
        {
            --floCameraLen_;
        }
    }
    vecDir = vecDir * (floLen + floCameraLen_);
    vecDir += XMLoadFloat3(&transform_.position_);
    XMStoreFloat3(&floDir_, vecDir);
    Camera::SetPosition(floDir_, padID_);
    Camera::SetTarget(transform_.position_, padID_);
}

void PlayerBase::PlayerFall()
{
    // �v���C���[���������鏈��
    if (isJump_)
    {
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - gravity_;
        positionPrevY_ = positionTempY_;
        IsJump();
    }
}

void PlayerBase::PlayerMove()
{
    const float walkSpeed = 0.4f;
    const float runSpeed = 0.5f;
    // �v���C���[�̈ړ�����
    if (!isRun_)
    {
        controllerMoveSpeed_ = XMFLOAT3(walkSpeed ,0.0f,walkSpeed);
    }
    else
    {
        controllerMoveSpeed_ = XMFLOAT3(runSpeed, 0.0f, runSpeed);
    }
    //�����ύX
    XMFLOAT3 m;
    XMStoreFloat3(&m, vecMove_);
    transform_.rotate_.y = XMConvertToDegrees(atan2(m.x, m.z));
    angle_ = XMConvertToDegrees(atan2(m.x, m.z));

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
    if (Input::GetPadStickL(padID_).y > deadZone)   //�O�ւ̈ړ�
    {
        transform_.position_.x += controllerMoveSpeed_.x;
        transform_.position_.z += controllerMoveSpeed_.z;
    }
    if (Input::GetPadStickL(padID_).y < -deadZone)  //���ւ̈ړ�
    {
        transform_.position_.x -= controllerMoveSpeed_.x;
        transform_.position_.z -= controllerMoveSpeed_.z;
    }
    if (Input::GetPadStickL(padID_).x > deadZone)   //�E�ւ̈ړ�
    {
        controllerMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        controllerMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        transform_.position_.x += controllerMoveSpeed_.x;
        transform_.position_.z += controllerMoveSpeed_.z;
    }
    if (Input::GetPadStickL(padID_).x < -deadZone)  //���ւ̈ړ�
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
        PlayerStun(getUpTime_);
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
    XMVECTOR vecPos;
    vecPos = XMLoadFloat3(&transform_.position_);
    return vecPos; // ���̖߂�l
}

void PlayerBase::IsMove()
{
    // �v���C���[���ړ������ǂ����𔻒肷�鏈��
    if (transform_.position_.x != positionPrev_.x)
        isMove_ = true;
    else if (transform_.position_.z != positionPrev_.z)
        isMove_ = true;
    else
        isMove_ = false;
}

void PlayerBase::IsJump()
{
    isJump_ = (rayStageDistDown_ + positionY_ > isFling_ && !isOnFloor_) ? true : isJump_;
    isJump_ = (positionY_ <= -rayFloorDistDown_ + playerInitPosY_) ? false : isJump_;
    isJump_ = (positionY_ <= -rayStageDistDown_ + playerInitPosY_) ? false : isJump_;
}

void PlayerBase::IsRun()
{
    isRun_ = (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, padID_) && !isJump_ && isMove_);
}

void PlayerBase::IsStun()
{

}

void PlayerBase::IsDive()
{
    //isDive_ = Input::GetPadTrrigerR(padID_) ? true : isDive_;
    if (Input::GetPadTrrigerR(padID_))
    {
        isDive_ = true;
    }
}
