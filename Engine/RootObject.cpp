#include "RootObject.h"
#include "SceneManager.h"


RootObject::RootObject():
	GameObject(nullptr, "RootObject")
{
}


RootObject::~RootObject()
{
}

void RootObject::Initialize()
{
	Instantiate<SceneManager>(this);
}

void RootObject::Update()
{
}

void RootObject::BothViewDraw()
{
}

void RootObject::LeftViewDraw()
{
}

void RootObject::RightViewDraw()
{
}

void RootObject::Release()
{
}
