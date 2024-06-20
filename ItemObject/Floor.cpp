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
    FloorUp(positionUpMax_);
    FloorDown(positionDownMax_);
}

void Floor::BothViewDraw()
{
    for (int i = initZeroInt; i <= 1; i++)
    {
        Model::SetTransform(hModel_, transform_);
        Model::Draw(hModel_);
    }
}


void Floor::LeftViewDraw()
{
}

void Floor::RightViewDraw()
{
}

void Floor::UPSubViewDraw()
{
}

void Floor::FloorUp(float _positionUpMax)
{
    if (transform_.position_.y >= _positionUpMax)
    {
        floorState_ = (int)FLOORSTATE::DOWN;
    }
}

void Floor::FloorDown(float _positionDownMax)
{
    if (transform_.position_.y <= _positionDownMax)
    {
        floorState_ = (int)FLOORSTATE::UP;
    }
}

void Floor::Release()
{
}

