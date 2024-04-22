//�C���N���[�h
#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../StageObject/Stage.h"
#include "../Player/AttackPlayer.h"
#include "ItemObjectManager.h"
#include "WoodBox.h"

WoodBox::WoodBox(GameObject* _pParent)
    :ItemObjectBase(_pParent, woodBoxName), hModel_{ -1 }, hSound_{ -1 },soundVolume_{0.3f}
    , isBreak_{ false }, woodBoxs_{}, gravity_{ 0.007f },woodBoxInitposY_{0.6}, positionY_{0.0f}, positionPrevY_{0.0f}, positionTempY_{0.0f}
    , isJump_{ false },isOnWoodBox_ {false}, rayWoodBoxDist_{ 0.0f }, rayStageDistDown_{ 0.0f }, isFling_{ 0.6f }
    ,pParent_{nullptr},pPlayScene_{nullptr}, pAttackPlayer_{ nullptr },pCollision_{nullptr}
{
    pParent_ = _pParent;
}

WoodBox::~WoodBox()
{ 

}

void WoodBox::Initialize()
{
    //�T�E���h�f�[�^�̃��[�h
    std::string soundName = soundFolderName + soundWoodBoxName + soundModifierName;
    hSound_ = Audio::Load(soundName);
    assert(hSound_ >= initZeroInt);
    //���f���f�[�^�̃��[�h
    std::string modelName = modelFolderName + woodBoxName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    pCollision_ = new SphereCollider(initZeroXMFLOAT3, 3.0f);
    AddCollider(pCollision_);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
    positionY_ = transform_.position_.y;
}

void WoodBox::Update()
{
    //transform_.position_.y = positionY_;
    positionY_ = transform_.position_.y;
    woodBoxs_ = pPlayScene_->GetWoodBoxs();
    WoodBoxFall();
    WoodBoxMove();
    WoodBoxRayCast();
    if (isBreak_)
    {
        WoodBoxDeath();
    }
}

void WoodBox::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void WoodBox::Release()
{
}

void WoodBox::WoodBoxFall()
{
    if (isJump_)
    {
        //�������ɉ����鏈��
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - gravity_;
        positionPrevY_ = positionTempY_;
        isJump_ = (isFling_ <= -rayStageDistDown_ + woodBoxInitposY_) ? false : isJump_;
    }
}

void WoodBox::WoodBoxMove()
{
}

void WoodBox::WoodBoxRayCast()
{
    RayCastData woodBoxDataDown;
    RayCastData stageDataDown;
    int woodBoxHModelStart     = woodBoxs_.front();
    int woodBoxHModelEnd       = woodBoxs_.back();
    int woodBoxHModelNow     = GetModelHandle();
    Stage* pStage           = (Stage*)FindObject(stageName);      //�X�e�[�W�I�u�W�F�N�g
    int stageHModel         = pStage->GetModelHandle();         //���f���ԍ����擾

    //���X�e�[�W�̖@��(�n�ʂɒ���t��)
    stageDataDown.start = transform_.position_;         //���C�̔��ˈʒu
    XMStoreFloat3(&stageDataDown.dir, vecDown);         //���C�̕���
    Model::RayCast(stageHModel, &stageDataDown);        //���C�𔭎�
    rayStageDistDown_ = stageDataDown.dist + woodBoxInitposY_;

    if (rayStageDistDown_ <= isFling_)
    {
        if (!isJump_ && !isOnWoodBox_)
        {
            positionY_ += -rayStageDistDown_;  //�n�ʂ̒���t��
            
            positionTempY_ = positionY_;
            positionPrevY_ = positionTempY_;
        }
    }
    if(rayStageDistDown_ >= isFling_ && !isOnWoodBox_)
    {
        isJump_ = true;
    }
    positionY_ = transform_.position_.y;
}

void WoodBox::WoodBoxDeath()
{
    Audio::Play(hSound_, soundVolume_);
    this->KillMe();
}

void WoodBox::OnCollision(GameObject* _pTarget)
{

}
