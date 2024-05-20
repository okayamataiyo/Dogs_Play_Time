#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/Global.h"
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
    std::string ModelName = modelFolderName + stageBlockName + modelModifierName;
    hModel_ = Model::Load(ModelName);
    assert(hModel_ >= 0);
    transform_.scale_ = { 2,1,2 };
    transform_.position_.y = -32.0f;
}

void StageBlock::Update()
{

}

void StageBlock::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void StageBlock::Release()
{
}