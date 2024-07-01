#pragma once

namespace
{
	enum class TREE
	{
		READY = 0,
		SUCCESS,
		FAILURE,
		RUNNING,
	};
}

//AIのアルゴリズムの1つ
class BehaviourTreeBase
{
protected:

public:
	BehaviourTreeBase()

	{

	}

	//デストラクタ
	virtual ~BehaviourTreeBase() {};
	virtual TREE Action() = 0;
	virtual TREE Selector() = 0;
	virtual TREE Sequence() = 0;
	virtual TREE Decorator() = 0;

};