#pragma once
#include <string>
#include <vector>
#include "GameObject.h"

//呼ばれたことを順番に記録するGameObject
//Instantiateは「親」だけを渡して作るので、名前はクラスごとに決めておく（ObjA、ObjB…）
class TestObject : public GameObject
{
public:
	//記録先（テストごとにLogScopeで用意する）
	inline static std::vector<std::string>* pLog = nullptr;

	TestObject(GameObject* pParent, const std::string& name)
		:GameObject(pParent, name)
	{
	}

	~TestObject() override { Record("Destroyed"); }
	void Initialize() override { Record("Initialize"); }
	void Update() override { Record("Update"); }
	void BothViewDraw() override { Record("Both"); }
	void LeftViewDraw() override { Record("Left"); }
	void RightViewDraw() override { Record("Right"); }
	void UPSubViewDraw() override { Record("UPSub"); }
	void Release() override { Record("Release"); }
	void OnCollision(GameObject* pTarget) override { Record("Hit:" + pTarget->GetObjectName()); }

private:
	void Record(const std::string& what)
	{
		if (pLog != nullptr)	pLog->push_back(GetObjectName() + ":" + what);
	}
};

template <char NAME>
class NamedObject : public TestObject
{
public:
	NamedObject(GameObject* pParent) :TestObject(pParent, std::string(1, NAME)) {}
};
using ObjA = NamedObject<'A'>;
using ObjB = NamedObject<'B'>;
using ObjC = NamedObject<'C'>;
using ObjD = NamedObject<'D'>;

//木の根（本物のRootObjectの代わり）
//テストが終わるとき、本物のMainと同じように、子孫をReleaseSubで解放する
class TestRoot : public TestObject
{
	bool isReleased_ = false;

public:
	TestRoot() :TestObject(nullptr, "root") {}

	//子孫を解放する（2回呼んでも1回だけ行う）
	void ReleaseAll()
	{
		if (isReleased_)	return;
		isReleased_ = true;
		ReleaseSub();
	}

	~TestRoot() override { ReleaseAll(); }
};

//記録先を用意する。スコープを出たら記録先を外す
//テストの中でいちばん先に作ること（オブジェクトより長く生きている必要がある）
class LogScope
{
public:
	std::vector<std::string> log;

	LogScope() { TestObject::pLog = &log; }
	~LogScope() { TestObject::pLog = nullptr; }

	void Clear() { log.clear(); }

	//記録を1つの文字列にする（失敗したときに読みやすいように、比べるのはこの文字列）
	std::string Text() const
	{
		std::string text;
		for (const std::string& entry : log)
		{
			if (!text.empty())	text += " ";
			text += entry;
		}
		return text;
	}
};
