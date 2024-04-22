#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "StageBlock.h"

StageBlock::StageBlock(GameObject* _parent)
    :StageObjectBase(_parent, stageBlockName), hModel_{ -1 }
{

}

StageBlock::~StageBlock()
{

}

void StageBlock::Initialize()
{
    //モデルデータのロード[
    std::string ModelName = (std::string)"Model&Picture/" + stageBlockName + (std::string)".fbx";
    hModel_ = Model::Load(ModelName);
    assert(hModel_ >= 0);
    //aaaaaaaaatransform_.position_.y = -10;
    //transform_.rotate_.x= 90;
    transform_.scale_ = { 2,1,2 };
    transform_.position_.y = -32.0f;
}

void StageBlock::Update()
{

}

void StageBlock::Draw()
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

void StageBlock::Release()
{
}