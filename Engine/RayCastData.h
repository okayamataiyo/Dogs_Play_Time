#pragma once
#include <Windows.h>
#include "Transform.h"

//レイキャスト用構造体
struct RayCastData
{
	XMFLOAT3	start;	//レイ発射位置
	XMFLOAT3	dir;	//レイの向きベクトル
	float       dist;	//衝突点までの距離
	BOOL        hit;	//レイが当たったか
	XMFLOAT3 normal;	//法線

	RayCastData() { dist = 99999.0f; }
};
