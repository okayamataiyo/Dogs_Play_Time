#include "Engine/Direct3D.h"
#include "Engine/Model.h"
#include "Engine/Image.h"
#include "Button.h"

Button::Button(GameObject* _pParent)
	:GameObject(_pParent, "Button"),hModel_{-1},hPushModel_{-1}, pushed_(0)
{
}

Button::~Button()
{
}

void Button::Initialize()
{
	center_ = { 0,0,0 };
	size_ = { 0,0,0 };
	hModel_ = Image::Load("Model&Picture/FoodButtonGive.png");
	hPushModel_ = Image::Load("Model&Picture/FoodButtonGiveDisable.png");
	size_ = Image::GetSize(hModel_);
}

void Button::Update()
{
}

void Button::Draw()
{
	if (pushed_) {
		Image::SetTransform(hPushModel_, transform_);
		Image::Draw(hPushModel_);
	}
	else {
		Image::SetTransform(hModel_, transform_);
		Image::Draw(hModel_);
	}
}

void Button::Release()
{
}

void Button::SetPosition(int _x, int _y)
{
	transform_.position_.x = (float)(_x - Direct3D::screenWidth_ / 2) / Direct3D::screenWidth_;
	transform_.position_.y = -(float)(_y - Direct3D::screenHeight_ / 2) / (Direct3D::screenHeight_ / 2);
	center_ = XMFLOAT3(_x, _y, 0);
}

bool Button::MouseInArea(XMFLOAT3 _mousePos)
{
	if (abs(_mousePos.x - center_.x) > size_.x / 2)
		return false;
	if (abs(_mousePos.y - center_.y) > size_.y / 2)
		return false;
	return true;
}

void Button::ForceMoveEnd()
{
	if (IsMoving())
	{
		SetPosition(endPos_.x, endPos_.y);
		currentTime_ = totalTime_;
	}
}
