//�C���N���[�h
#include "../Engine/Global.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/ImGui/imgui.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "../StageObject/Stage.h"
#include "ActorCollectPlayer.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"

ActorCollectPlayer::ActorCollectPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, actorCollectPlayerName), hModel_{ -1 }, stageHModel_{ -1 },isSelect_{false}, gameState_{GAMESTATE::READY}
    , pDogs_Walk_PlayScene_{ nullptr }, pStage_{ nullptr }
{
}

ActorCollectPlayer::~ActorCollectPlayer()
{
}

void ActorCollectPlayer::Initialize()
{
    //�����f���f�[�^�̃��[�h
    std::string modelName = modelFolderName + actorCollectPlayerName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    transform_.scale_ = { 0.4f,0.4f,0.4f };
    jumpData_.positionY_ = transform_.position_.y;
    pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
    pStage_ = (Stage*)FindObject(stageName);
}

void ActorCollectPlayer::Update()
{
    UpdatePlay();
}

void ActorCollectPlayer::PlayerStun(int _timeLimit)
{
}

void ActorCollectPlayer::BothViewDraw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void ActorCollectPlayer::LeftViewDraw()
{
}

void ActorCollectPlayer::RightViewDraw()
{}

void ActorCollectPlayer::UPSubViewDraw()
{
}

void ActorCollectPlayer::Release()
{
}

void ActorCollectPlayer::UpdateReady()
{
}

void ActorCollectPlayer::UpdatePlay()
{
    //���������̏���
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

void ActorCollectPlayer::UpdateGameOver()
{
}

void ActorCollectPlayer::OnCollision(GameObject* _pTarget)
{
}

void ActorCollectPlayer::PlayerScore()
{
}

void ActorCollectPlayer::PlayerCamera()
{
}

void ActorCollectPlayer::PlayerFall()
{
    if (jumpData_.isJump_)
    {
        //�������ɉ����鏈��
        jumpData_.positionTempY_ = jumpData_.positionY_;
        jumpData_.positionY_ += (jumpData_.positionY_ - jumpData_.positionPrevY_) - jumpData_.gravity_;
        jumpData_.positionPrevY_ = jumpData_.positionTempY_;
    }
}

void ActorCollectPlayer::PlayerMove()
{
    PlayerBase::PlayerMove();
    if (!isSelect_)
    {
        transform_.position_.x += moveData_.padMoveSpeed_.x;
        transform_.position_.z += moveData_.padMoveSpeed_.z;
    }
}

void ActorCollectPlayer::PlayerJump()
{
}

void ActorCollectPlayer::PlayerJumpPower()
{
    PlayerBase::PlayerJumpPower();
}

void ActorCollectPlayer::PlayerDive()
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

void ActorCollectPlayer::PlayerDivePower()
{
    //�Ƃт��̏���
    jumpData_.isJump_ = true;
    jumpData_.positionPrevY_ = jumpData_.positionY_;
    jumpData_.positionY_ += diveData_.divePower_;
}

void ActorCollectPlayer::PlayerKnockback()
{
}

void ActorCollectPlayer::PlayerRayCast()
{
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //���f���ԍ����擾
    //�����̖@��(�n�ʂɒ���t��)
    stageDataDown.start = transform_.position_;   //���C�̔��ˈʒu
    stageDataDown.start.y = initZeroFloat;
    XMStoreFloat3(&stageDataDown.dir, vecDown);   //���C�̕���
    Model::RayCast(stageHModel_, &stageDataDown); //���C�𔭎�
    wallData_.rayStageDistDown_ = stageDataDown.dist;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    if (wallData_.rayStageDistDown_ + jumpData_.positionY_ <= moveData_.isFling_)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (!jumpData_.isJump_ && !floorData_.isOnFloor_)
        {
            //�n�ʂɒ���t��
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
}

void ActorCollectPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
}

void ActorCollectPlayer::IsMove()
{
    PlayerBase::IsMove();
}

void ActorCollectPlayer::IsJump()
{
    PlayerBase::IsJump();
}

void ActorCollectPlayer::IsRun()
{
    PlayerBase::IsRun();
}

void ActorCollectPlayer::IsStun()
{
    PlayerBase::IsStun();
}

void ActorCollectPlayer::IsDive()
{
    PlayerBase::IsDive();
}
