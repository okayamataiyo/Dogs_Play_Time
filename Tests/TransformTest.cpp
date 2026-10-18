#include "TestUtil.h"
#include "doctest.h"
#include "Transform.h"

namespace
{
	//ワールド行列で点を変換する
	XMFLOAT3 Apply(Transform& transform, float x, float y, float z)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVector3TransformCoord(XMVectorSet(x, y, z, 1), transform.GetWorldMatrix()));
		return result;
	}

	void CheckPoint(const XMFLOAT3& actual, float x, float y, float z)
	{
		CHECK(actual.x == doctest::Approx(x).epsilon(1e-4));
		CHECK(actual.y == doctest::Approx(y).epsilon(1e-4));
		CHECK(actual.z == doctest::Approx(z).epsilon(1e-4));
	}

	void CheckMatrix(const XMMATRIX& actual, const XMMATRIX& expected)
	{
		XMFLOAT4X4 a, e;
		XMStoreFloat4x4(&a, actual);
		XMStoreFloat4x4(&e, expected);
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				CHECK(a.m[row][column] == doctest::Approx(e.m[row][column]).epsilon(1e-4));
			}
		}
	}
}

TEST_CASE("Transform: the default is no move, no rotation, scale 1, no parent")
{
	Transform transform;
	CHECK(transform.position_.x == 0);
	CHECK(transform.position_.y == 0);
	CHECK(transform.position_.z == 0);
	CHECK(transform.rotate_.x == 0);
	CHECK(transform.rotate_.y == 0);
	CHECK(transform.rotate_.z == 0);
	CHECK(transform.scale_.x == 1);
	CHECK(transform.scale_.y == 1);
	CHECK(transform.scale_.z == 1);
	CHECK(transform.pParent_ == nullptr);
	CHECK(XMMatrixIsIdentity(transform.GetWorldMatrix()));
}

TEST_CASE("Transform: position moves the origin")
{
	Transform transform;
	transform.position_ = XMFLOAT3(1, 2, 3);
	CheckPoint(Apply(transform, 0, 0, 0), 1, 2, 3);
}

TEST_CASE("Transform: scale is applied before the move")
{
	Transform transform;
	transform.scale_ = XMFLOAT3(2, 3, 4);
	transform.position_ = XMFLOAT3(1, 0, 0);
	CheckPoint(Apply(transform, 1, 1, 1), 3, 3, 4);
}

TEST_CASE("Transform: rotate_ is in degrees, and 90 degrees around Y turns +Z into +X")
{
	Transform transform;
	transform.rotate_ = XMFLOAT3(0, 90, 0);
	CheckPoint(Apply(transform, 0, 0, 1), 1, 0, 0);
}

TEST_CASE("Transform: the rotation matrix is Z, then X, then Y")
{
	Transform transform;
	transform.rotate_ = XMFLOAT3(30, 40, 50);
	transform.Calclation();

	XMMATRIX expected = XMMatrixRotationZ(XMConvertToRadians(50)) * XMMatrixRotationX(XMConvertToRadians(30)) * XMMatrixRotationY(XMConvertToRadians(40));
	CheckMatrix(transform.matRotate_, expected);
}

TEST_CASE("Transform: a child follows its parent's move and scale")
{
	Transform parent;
	parent.position_ = XMFLOAT3(0, 0, 5);
	parent.scale_ = XMFLOAT3(2, 2, 2);

	Transform child;
	child.position_ = XMFLOAT3(1, 0, 0);
	child.pParent_ = &parent;

	//子の位置(1,0,0)が親の拡大で(2,0,0)になり、親の位置だけ動く
	CheckPoint(Apply(child, 0, 0, 0), 2, 0, 5);
}

TEST_CASE("Transform: a chain of parents is followed all the way up")
{
	Transform grandParent;
	grandParent.position_ = XMFLOAT3(10, 0, 0);

	Transform parent;
	parent.position_ = XMFLOAT3(0, 20, 0);
	parent.pParent_ = &grandParent;

	Transform child;
	child.position_ = XMFLOAT3(0, 0, 30);
	child.pParent_ = &parent;

	CheckPoint(Apply(child, 0, 0, 0), 10, 20, 30);
}

TEST_CASE("Transform::Float3Add: adds each element")
{
	XMFLOAT3 sum = Transform::Float3Add(XMFLOAT3(1, 2, 3), XMFLOAT3(10, 20, 30));
	CHECK(sum.x == 11);
	CHECK(sum.y == 22);
	CHECK(sum.z == 33);
}
