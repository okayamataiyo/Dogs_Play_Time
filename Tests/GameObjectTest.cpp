#include "TestUtil.h"
#include "doctest.h"
#include "Direct3D.h"
#include "TestObjects.h"

//GameObjectの親子関係・更新・描画・解放の順序を、今の動きのまま確かめる
//（所有権の持ち方を変えても、この順序が変わらないことを確かめるためのテスト）

namespace
{
	//解放されたコライダーを数える
	class CountedBox : public BoxCollider
	{
	public:
		inline static int destroyedCount = 0;

		CountedBox() :BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)) {}
		~CountedBox() override { destroyedCount++; }
	};
}

TEST_CASE("GameObject: Instantiate adds the child to the end of the list, InstantiateFront to the front, and Initialize is called once")
{
	LogScope scope;
	TestRoot root;

	ObjA* a = Instantiate<ObjA>(&root);
	ObjB* b = Instantiate<ObjB>(&root);
	ObjC* c = InstantiateFront<ObjC>(&root);

	CHECK(scope.Text() == "A:Initialize B:Initialize C:Initialize");
	CHECK(a->GetParent() == &root);

	std::vector<GameObject*> children(root.GetChildList()->begin(), root.GetChildList()->end());
	REQUIRE(children.size() == 3);
	CHECK(children[0] == c);
	CHECK(children[1] == a);
	CHECK(children[2] == b);
}

TEST_CASE("GameObject: UpdateSub updates a parent before its children, depth first, in list order")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	Instantiate<ObjB>(a);
	Instantiate<ObjC>(&root);
	scope.Clear();

	root.UpdateSub();
	CHECK(scope.Text() == "root:Update A:Update B:Update C:Update");
}

TEST_CASE("GameObject: a child that called KillMe is still updated that time, then released from its own children up, and removed from the list")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	Instantiate<ObjB>(a);
	scope.Clear();

	CHECK_FALSE(a->IsDead());
	a->KillMe();
	CHECK(a->IsDead());

	root.UpdateSub();
	CHECK(scope.Text() == "root:Update A:Update B:Update B:Release B:Destroyed A:Release A:Destroyed");
	CHECK(root.GetChildList()->empty());
}

TEST_CASE("GameObject: ReleaseSub releases each subtree from its leaves up, children in list order, then the object itself")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	Instantiate<ObjD>(a);
	Instantiate<ObjB>(&root);
	scope.Clear();

	root.ReleaseAll();
	CHECK(scope.Text() == "D:Release D:Destroyed A:Release A:Destroyed B:Release B:Destroyed root:Release");
}

TEST_CASE("GameObject: KillAllChildren deletes every descendant from its leaves up, and leaves the object itself")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	Instantiate<ObjD>(a);
	Instantiate<ObjB>(&root);
	scope.Clear();

	root.KillAllChildren();
	CHECK(scope.Text() == "D:Release D:Destroyed A:Release A:Destroyed B:Release B:Destroyed");
	CHECK(root.GetChildList()->empty());
}

TEST_CASE("GameObject: FindChildObject looks at descendants only, FindObject looks at the whole tree from the root")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	ObjD* d = Instantiate<ObjD>(a);
	ObjB* b = Instantiate<ObjB>(&root);

	CHECK(root.FindChildObject("D") == d);
	CHECK(root.FindChildObject("missing") == nullptr);
	CHECK(a->FindChildObject("B") == nullptr);
	CHECK(d->FindObject("B") == b);
}

TEST_CASE("GameObject: DrawSub draws a parent before its children, and picks the draw functions by viewport")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	Instantiate<ObjB>(a);

	Direct3D::viewPortType_ = (int)Direct3D::VIEWPORTSTATE::LEFTVIEWPORT;
	scope.Clear();
	root.DrawSub();
	CHECK(scope.Text() == "root:Both root:Left A:Both A:Left B:Both B:Left");

	Direct3D::viewPortType_ = (int)Direct3D::VIEWPORTSTATE::RIGHTVIEWPORT;
	scope.Clear();
	root.DrawSub();
	CHECK(scope.Text() == "root:Both root:Right A:Both A:Right B:Both B:Right");

	Direct3D::viewPortType_ = (int)Direct3D::VIEWPORTSTATE::UPSUBVIEWPORT;
	scope.Clear();
	root.DrawSub();
	CHECK(scope.Text() == "root:UPSub A:UPSub B:UPSub");

	Direct3D::viewPortType_ = 0;
}

TEST_CASE("GameObject: objects under a scene get OnCollision when their colliders overlap, once per update")
{
	LogScope scope;
	TestRoot root;
	ObjC* scene = Instantiate<ObjC>(&root);		//本物のゲームと同じく、当たり判定の相手は「同じ親の下」にいる
	ObjA* a = Instantiate<ObjA>(scene);
	ObjB* b = Instantiate<ObjB>(scene);
	a->AddCollider(std::make_unique<BoxCollider>(XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 2)));
	b->AddCollider(std::make_unique<BoxCollider>(XMFLOAT3(0, 0, 0), XMFLOAT3(2, 2, 2)));
	a->SetPosition(0, 0, 0);
	b->SetPosition(1, 0, 0);
	scope.Clear();

	root.UpdateSub();
	CHECK(scope.Text() == "root:Update C:Update A:Update B:Update A:Hit:B B:Hit:A");

	//離れたら当たらない
	b->SetPosition(10, 0, 0);
	scope.Clear();
	root.UpdateSub();
	CHECK(scope.Text() == "root:Update C:Update A:Update B:Update");
}

TEST_CASE("GameObject: colliders are deleted by ClearCollider, by ReleaseSub and by the destructor")
{
	LogScope scope;
	CountedBox::destroyedCount = 0;
	{
		TestRoot root;
		ObjA* a = Instantiate<ObjA>(&root);
		a->AddCollider(std::make_unique<CountedBox>());
		a->AddCollider(std::make_unique<CountedBox>());
		a->ClearCollider();
		CHECK(CountedBox::destroyedCount == 2);

		ObjB* b = Instantiate<ObjB>(&root);
		b->AddCollider(std::make_unique<CountedBox>());
		root.ReleaseAll();
		CHECK(CountedBox::destroyedCount == 3);
	}

	//ReleaseSubを通さずに消した場合は、デストラクタが消す
	ObjA* lone = new ObjA(nullptr);
	lone->AddCollider(std::make_unique<CountedBox>());
	delete lone;
	CHECK(CountedBox::destroyedCount == 4);
}

TEST_CASE("GameObject: AddCollider takes over the collider and returns it")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);

	std::unique_ptr<SphereCollider> sphere = std::make_unique<SphereCollider>(XMFLOAT3(0, 0, 0), 1.0f);
	SphereCollider* raw = sphere.get();
	CHECK(a->AddCollider(std::move(sphere)) == raw);
	CHECK(sphere == nullptr);
}

TEST_CASE("GameObject: the flags start as not initialized, entered, visible, alive, and can be changed")
{
	LogScope scope;
	TestRoot root;
	CHECK_FALSE(root.IsInitialized());
	CHECK(root.IsEntered());
	CHECK(root.IsVisibled());
	CHECK_FALSE(root.IsDead());

	root.SetInitialized();
	root.Leave();
	root.Invisible();
	CHECK(root.IsInitialized());
	CHECK_FALSE(root.IsEntered());
	CHECK_FALSE(root.IsVisibled());

	root.Enter();
	root.Visible();
	CHECK(root.IsEntered());
	CHECK(root.IsVisibled());
}

TEST_CASE("GameObject: the world position is the parent's position plus its own (the parent's parent is not added)")
{
	LogScope scope;
	TestRoot root;
	ObjA* a = Instantiate<ObjA>(&root);
	ObjB* b = Instantiate<ObjB>(a);

	root.SetPosition(10, 20, 30);
	a->SetPosition(1, 2, 3);
	b->SetPosition(100, 0, 0);

	XMFLOAT3 positionA = a->GetWorldPosition();
	CHECK(positionA.x == 11);
	CHECK(positionA.y == 22);
	CHECK(positionA.z == 33);

	//Bには、親Aの位置だけが足される（Aの親のrootの位置は足されない）
	XMFLOAT3 positionB = b->GetWorldPosition();
	CHECK(positionB.x == 101);
	CHECK(positionB.y == 2);
	CHECK(positionB.z == 3);
}
