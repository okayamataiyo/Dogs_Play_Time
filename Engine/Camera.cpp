#include "Camera.h"
#include "Direct3D.h"
#include "Global.h"

XMFLOAT3 position_[2];
XMFLOAT3 target_[2];
XMMATRIX view_;
XMMATRIX proj_;
XMMATRIX billBoard_;
float one_ViewAsp_ = 1;
float both_ViewAsp_ = 2;
int isChangeView_ = (int)Direct3D::VIEWSTATE::LEFTVIEW;
float vPSize_ = 1;
float viewChangeSpeed_ = 0.008f;

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	for (int i = 0u; i <= 1; i++)
	{
		position_[i] = XMFLOAT3(0, 3 * i, -20);	//カメラの位置
		target_[i] = XMFLOAT3(0, 0, 0);	//カメラの焦点
	}
}

//更新（ビュー行列作成）
void Camera::Update(int _type)
{
	switch (isChangeView_)
	{
	case (int)Direct3D::VIEWSTATE::LEFTVIEW:
		if (vPSize_ >= one_ViewAsp_)
		{
			vPSize_ -= viewChangeSpeed_;
		}
		break;
	case (int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW:
		if (vPSize_ <= both_ViewAsp_)
		{
			vPSize_ += viewChangeSpeed_;
		}
		break;
	case (int)Direct3D::VIEWSTATE::RIGHTVIEW:
		if (vPSize_ >= one_ViewAsp_)
		{
			vPSize_ -= viewChangeSpeed_;
		}
		break;
	case (int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW:
		if (vPSize_ <= both_ViewAsp_)
		{
			vPSize_ += viewChangeSpeed_;
		}
		break;
	}
	//プロジェクション行列
	if (Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW || Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW)
	{
		proj_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)(Direct3D::screenWidth_ / vPSize_) / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
	}
	if (Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::LEFTVIEW || Direct3D::GetIsChangeView() == (int)Direct3D::VIEWSTATE::RIGHTVIEW)
	{
		proj_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)(Direct3D::screenWidth_ / vPSize_) / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
	}

	//ビュー行列
	view_ = XMMatrixLookAtLH(XMVectorSet(position_[_type].x, position_[_type].y, position_[_type].z, 0),
		XMVectorSet(target_[_type].x, target_[_type].y, target_[_type].z, 0), XMVectorSet(0, 1, 0, 0));

	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	billBoard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&target_[_type])- XMLoadFloat3(&position_[_type]), XMVectorSet(0, 1, 0, 0));
	billBoard_ = XMMatrixInverse(nullptr, billBoard_);
}

//焦点を設定
void Camera::SetTarget(XMFLOAT3 _target,int _type) { target_[_type] = _target; }

void Camera::SetIsChangeView(int _isChangeView)
{
	isChangeView_ = _isChangeView;
}

//位置を設定
void Camera::SetPosition(XMFLOAT3 _position, int _type) { position_[_type] = _position; }

//焦点を取得
XMFLOAT3 Camera::GetTarget(int _type) { return target_[_type]; }

XMVECTOR Camera::VecGetTarget(int _type) { return XMLoadFloat3(&target_[_type]); }

//位置を取得
XMFLOAT3 Camera::GetPosition(int _type) { return position_[_type]; }

XMVECTOR Camera::VecGetPosition(int _type) { return XMLoadFloat3(&position_[_type]); }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return view_; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return proj_; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix(){	return billBoard_; }
