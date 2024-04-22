#include "Sky.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"

Sky::Sky(GameObject* _parent)
	:StageObjectBase(_parent, skyName), hModel_(-1)
{

}

Sky::~Sky()
{
}

void Sky::Initialize()
{
	//モデルデータのロード
	std::string ModelName = (std::string)"Model&Picture/" + skyName + (std::string)".fbx";
	hModel_ = Model::Load(ModelName);
	assert(hModel_ >= 0);
	transform_.scale_ = { 500.0f,500.0f,500.0f };
	transform_.position_.y = -50.0f;
	transform_.rotate_.x = 90.0f;
}

void Sky::Update()
{
	transform_.rotate_.y += 0.01;
}

void Sky::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_SKY);
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Sky::Release()
{
}
