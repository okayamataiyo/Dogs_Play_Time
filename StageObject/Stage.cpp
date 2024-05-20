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
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Model&Picture/NewStage.fbx");
    assert(hModel_ >= 0);
    transform_.scale_ = { 2,1,2 };
}

void Stage::Update()
{

}

void Stage::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Stage::Release()
{
}