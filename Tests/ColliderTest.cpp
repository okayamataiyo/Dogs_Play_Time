#include "TestUtil.h"
#include "doctest.h"
#include "TestObjects.h"

//箱型と球体の当たり判定を、今の動きのまま確かめる
//箱のサイズは全体の幅、球体のサイズは半径。位置は「親の位置＋自分の位置＋判定の中心」
namespace
{
	//rootの子としてオブジェクトを作り、判定を付ける
	//判定の所有はオブジェクトに移るので、返したポインターは使うだけ
	BoxCollider* AddBox(GameObject* object, XMFLOAT3 center, XMFLOAT3 size)
	{
		BoxCollider* box = new BoxCollider(center, size);
		object->AddCollider(box);
		return box;
	}

	SphereCollider* AddSphere(GameObject* object, XMFLOAT3 center, float radius)
	{
		SphereCollider* sphere = new SphereCollider(center, radius);
		object->AddCollider(sphere);
		return sphere;
	}

	//どちらの側から調べても同じ結果になる
	bool IsHit(Collider* a, Collider* b)
	{
		bool ab = a->IsHit(b);
		bool ba = b->IsHit(a);
		CHECK(ab == ba);
		return ab;
	}
}

TEST_CASE("Collider: box vs box hits when they overlap, and not when they only touch")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	ObjB* b = Instantiate<ObjB>(&root);
	Collider* boxA = AddBox(a, XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 2));
	Collider* boxB = AddBox(b, XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 2));

	b->SetPosition(1.5f, 0, 0);
	CHECK(IsHit(boxA, boxB));

	b->SetPosition(2, 0, 0);		//面がちょうど触れているだけ
	CHECK_FALSE(IsHit(boxA, boxB));

	b->SetPosition(3, 0, 0);
	CHECK_FALSE(IsHit(boxA, boxB));

	b->SetPosition(0, 1.5f, 1.5f);	//他の軸でも同じ
	CHECK(IsHit(boxA, boxB));
	b->SetPosition(0, 0, 2.5f);
	CHECK_FALSE(IsHit(boxA, boxB));
}

TEST_CASE("Collider: the collider's center and the parent's position move where it hits")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	ObjB* b = Instantiate<ObjB>(&root);
	Collider* boxA = AddBox(a, XMFLOAT3(5, 0, 0), XMFLOAT3(2, 2, 2));		//Aの判定は原点から5ずれた所
	Collider* boxB = AddBox(b, XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 2));

	CHECK_FALSE(IsHit(boxA, boxB));		//AとBの位置は同じだが、判定がずれている

	b->SetPosition(5, 0, 0);
	CHECK(IsHit(boxA, boxB));

	root.SetPosition(100, 0, 0);		//親の位置は、両方に同じだけ足される
	CHECK(IsHit(boxA, boxB));
}

TEST_CASE("Collider: sphere vs sphere hits when the distance is at most the sum of the radii")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	ObjB* b = Instantiate<ObjB>(&root);
	Collider* sphereA = AddSphere(a, XMFLOAT3(0, 0, 0), 1.0f);
	Collider* sphereB = AddSphere(b, XMFLOAT3(0, 0, 0), 1.0f);

	b->SetPosition(1, 1, 0);
	CHECK(IsHit(sphereA, sphereB));

	b->SetPosition(2, 0, 0);		//距離がちょうど半径の和（球体どうしは、触れていれば当たる）
	CHECK(IsHit(sphereA, sphereB));

	b->SetPosition(2.01f, 0, 0);
	CHECK_FALSE(IsHit(sphereA, sphereB));
}

TEST_CASE("Collider: box vs sphere hits inside the box widened by the radius on every side, and not when only touching")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	ObjB* b = Instantiate<ObjB>(&root);
	Collider* box = AddBox(a, XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 2));
	Collider* sphere = AddSphere(b, XMFLOAT3(0, 0, 0), 1.0f);

	b->SetPosition(1.9f, 0, 0);
	CHECK(IsHit(box, sphere));

	b->SetPosition(2, 0, 0);		//広げた箱の面の上は、当たらない
	CHECK_FALSE(IsHit(box, sphere));

	//箱の角の外側でも、各軸が広げた箱の中なら当たる（球体と角の本当の距離は半径より遠い）
	b->SetPosition(1.9f, 1.9f, 1.9f);
	CHECK(IsHit(box, sphere));

	b->SetPosition(1.9f, 1.9f, 2.1f);
	CHECK_FALSE(IsHit(box, sphere));
}
