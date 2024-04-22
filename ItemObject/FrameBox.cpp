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
#include "FrameBox.h"
#include "ItemObjectManager.h"

FrameBox::FrameBox(GameObject* _pParent)
    :ItemObjectBase(_pParent, frameBoxName), hModel_{ -1 }, isBreak_{false}
    , frameBoxInitPosY_{2.0f}
    , pAttackPlayer_{nullptr},pPlayScene_{nullptr}
{
}

FrameBox::~FrameBox()
{

}

void FrameBox::Initialize()
{
    //モデルデータのロード
    std::string modelName;
    modelName = debugCollisionName + boxColliderName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
}

void FrameBox::Update()
{
    transform_.position_ = pPlayScene_->GetAttackPlayerPosition();
    transform_.position_.y = transform_.position_.y + frameBoxInitPosY_;
    transform_.rotate_.y = XMConvertToDegrees(pAttackPlayer_->GetAngle());
}

void FrameBox::Draw()
{
    Direct3D::SetShader(Direct3D::SHADER_UNLIT);
    Model::SetTransform(hModel_, transform_);
    if (!pAttackPlayer_->GetIsJump())
    {
        Model::Draw(hModel_);
    }
    Direct3D::SetShader(Direct3D::SHADER_3D);
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
