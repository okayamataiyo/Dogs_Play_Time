#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "Stage.h"

Stage::Stage(GameObject* _parent)
    :StageObjectBase(_parent, stageName), hModel_{ -1 }
{

}

Stage::~Stage()
{

}

void Stage::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Model&Picture/OldStage.fbx");
    assert(hModel_ >= 0);
    //aaaaaaaaatransform_.position_.y = -10;
    //transform_.rotate_.x= 90;
    transform_.scale_ = { 2,1,2 };
}

void Stage::Update()
{

}

void Stage::Draw()
{
    Model::SetTransform(hModel_, transform_);
    //最初に3Dで描画後、枠づけも描画
    //for (int j = 0; j <= 4; j += 4)
    //{
    //    Direct3D::SetShader(static_cast<Direct3D::SHADER_TYPE>(j));
    //    Model::Draw(hModel_);
    //}
    Model::Draw(hModel_);
}

void Stage::Release()
{
}