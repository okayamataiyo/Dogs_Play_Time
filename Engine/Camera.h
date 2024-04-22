#pragma once
#include "GameObject.h"

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize();

	//更新（ビュー行列作成）
	void Update(int _type);

	//視点（カメラの位置）を設定
	void SetPosition(XMFLOAT3 _position, int _type);

	//焦点（見る位置）を設定
	void SetTarget(XMFLOAT3 _target, int _type);

	void SetIsChangeView(int _isChangeView);

	//位置を取得
	XMFLOAT3 GetPosition(int _type);

	XMVECTOR VecGetPosition(int _type);

	//焦点を取得
	XMFLOAT3 GetTarget(int _type);

	XMVECTOR VecGetTarget(int _type);

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	//ビルボード用回転行列を取得
	XMMATRIX GetBillboardMatrix();
};