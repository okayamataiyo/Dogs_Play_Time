//インクルード
#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../Player/AttackPlayer.h"
#include "../StageObject/Stage.h"
#include "../Scene/Dogs_Walk_PlayScene.h"
#include "../Scene/Dogs_Fight_PlayScene.h"
#include "FrameBox.h"
#include "ItemObjectManager.h"

FrameBox::FrameBox(GameObject* _pParent)
    :ItemObjectBase(_pParent, frameBoxName), hModel_{ -1 }, isBreak_{false}
    , frameBoxInitPosY_{2.0f}, walkOrFight_{0}
    , pAttackPlayer_{nullptr},pDogs_Walk_PlayScene_{nullptr}
{
}

FrameBox::~FrameBox()
{

}

void FrameBox::Initialize()
{
    //▼INIファイルからデータのロード
    walkOrFight_ = GetPrivateProfileInt("PLAYSCENEID", "WalkOrFight", 0, "Setting/PlaySceneSetting.ini");
    //モデルデータのロード
    std::string modelName;
    modelName = debugCollisionFolderName + boxColliderName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    pDogs_Walk_PlayScene_ = (Dogs_Walk_PlayScene*)FindObject(Dogs_Walk_PlaySceneName);
    pDogs_Fight_PlayScene_ = (Dogs_Fight_PlayScene*)FindObject(Dogs_Fight_PlaySceneName);
    pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
}

void FrameBox::Update()
{
    if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSWALK)
    {
        transform_.position_ = pDogs_Walk_PlayScene_->GetAttackPlayerPosition();
    }
    if (walkOrFight_ == (int)PLAYSCENESTATE::DOGSFIGHT)
    {
        transform_.position_ = pDogs_Fight_PlayScene_->GetAttackPlayerPosition();
    }
    transform_.position_.y = transform_.position_.y + frameBoxInitPosY_;
    transform_.rotate_.y = pAttackPlayer_->GetAngle();
}

void FrameBox::BothViewDraw()
{
    Direct3D::SetShader(Direct3D::SHADER_UNLIT);
    Model::SetTransform(hModel_, transform_);
    if (pAttackPlayer_->GetIsJump())
    {
        Model::Draw(hModel_);
    }
    Direct3D::SetShader(Direct3D::SHADER_3D);
}

void FrameBox::LeftViewDraw()
{
}

void FrameBox::RightViewDraw()
{
}

void FrameBox::UPSubViewDraw()
{
}

void FrameBox::Release()
{
}

void FrameBox::Move()
{
}

void FrameBox::OnCollision(GameObject* _pTarget)
{

}
