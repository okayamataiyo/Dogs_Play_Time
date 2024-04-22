#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/Global.h"
#include "Floor.h"
#include "ItemObjectBase.h"

Floor::Floor(GameObject* _parent)
    :ItemObjectBase(_parent, floorName), hModel_{-1}, floorState_{(int)FLOORSTATE::DOWN}
    ,upVelocity_{0.1f},downVelocity_{0.1f},positionUpMax_{10.0f},positionDownMax_{0.0f}
{

}

Floor::~Floor()
{

}

void Floor::Initialize()
{
    //モデルデータのロード
    std::string ModelName;
    ModelName = modelFolderName + floorName + modelModifierName;
    hModel_ = Model::Load(ModelName);
    assert(hModel_ >= initZeroInt);
    transform_.scale_ = { 1,1,1 };
}

void Floor::Update()
{
    if (floorState_ == (int)FLOORSTATE::UP)
    {
        transform_.position_.y += upVelocity_;
    }
    if (floorState_ == (int)FLOORSTATE::DOWN)
    {
        transform_.position_.y -= downVelocity_;
    }

    if (transform_.position_.y >= positionUpMax_)
    {
        floorState_ = (int)FLOORSTATE::DOWN;
    }
    if (transform_.position_.y <= positionDownMax_)
    {
        floorState_ = (int)FLOORSTATE::UP;
    }
}

void Floor::Draw()
{
    for (int i = initZeroInt; i <= 1; i++)
    {
        Model::SetTransform(hModel_, transform_);
        Model::Draw(hModel_);
    }
}

void Floor::Release()
{
}

