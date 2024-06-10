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
    const float walkSpeed = 0.4f;
    const float runSpeed = 0.6f;
    // �v���C���[�̈ړ�����
    if (!moveData_.isRun_)
    {
        moveData_.padMoveSpeed_ = XMFLOAT3(walkSpeed ,0.0f,walkSpeed);
    }
    else
    {
        moveData_.padMoveSpeed_ = XMFLOAT3(runSpeed, 0.0f, runSpeed);
    }
    //�����ύX
    XMFLOAT3 m;
    XMStoreFloat3(&m, dirData_.vecMove_);
    transform_.rotate_.y = XMConvertToDegrees(atan2(m.x, m.z));
    dirData_.angle_ = XMConvertToDegrees(atan2(m.x, m.z));

    float pi = 3.14f;					//�~����
    float halfPi = pi / 2;				//�~�����̔���

    //XMConvertToRadians = degree�p��radian�p��(����)�ϊ�����
    //XMMatrixRotationY = Y���W�𒆐S�ɉ�]������s������֐�
    const XMMATRIX rotmat = XMMatrixRotationY(halfPi);
    dirData_.vecDirection_ = XMVectorSetY(dirData_.vecDirection_, 0);
    dirData_.vecDirection_ = XMVector3Normalize(dirData_.vecDirection_);

    const float deadZone = 0.3f;		//�R���g���[���[�̃f�b�g�]�[��
    const float plusDir = 1.0f;
    const float minusDir = -1.0f;
    moveData_.padMoveSpeed_.x *= XMVectorGetX(dirData_.vecDirection_);
    moveData_.padMoveSpeed_.z *= XMVectorGetZ(dirData_.vecDirection_);
    XMVECTOR tempvec = XMVector3Transform(dirData_.vecDirection_, rotmat);
    if (Input::GetPadStickL(gameData_.padID_).y > deadZone)   //�O�ւ̈ړ�
    {
        ApplyMovement(plusDir, plusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).y < -deadZone)  //���ւ̈ړ�
    {
        ApplyMovement(minusDir, minusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).x > deadZone)   //�E�ւ̈ړ�
    {
        moveData_.padMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        moveData_.padMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        ApplyMovement(plusDir, plusDir);
    }
    if (Input::GetPadStickL(gameData_.padID_).x < -deadZone)  //���ւ̈ړ�
    {
        moveData_.padMoveSpeed_.x = 0.3f * XMVectorGetX(tempvec);
        moveData_.padMoveSpeed_.z = 0.3f * XMVectorGetZ(tempvec);
        ApplyMovement(minusDir, minusDir);
    }

    const float outerWallPosFront = 99.0f;		//�O�̊O�ǂ̈ʒu
    const float outerWallPosBack = -99.0f;		//���̊O�ǂ̈ʒu
    const float outerWallPosLeft = 99.0f;		//���̊O�ǂ̈ʒu
    const float outerWallPosRight = -99.0f;		//�E�̊O�ǂ̈ʒu

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
    if (Input::GetPadTrrigerR(gameData_.padID_))
    {
        diveData_.isDive_ = true;
    }
}
