#pragma once
class BehaviourTreeManager;

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
	BehaviourTreeManager* pBehaviourTreeManager_;
public:
	BehaviourTreeBase(BehaviourTreeManager* _manager)
		:pBehaviourTreeManager_(_manager)
	{

	}

	//デストラクタ
	virtual ~BehaviourTreeBase() {};
	//状態に入る時の初期化
	virtual void EnterState() = 0;
	//状態の更新
	virtual void UpdateState() = 0;
	//状態から出る時の後処理
	virtual void ExitState() = 0;
	virtual TREE Action() = 0;
	virtual TREE Selector() = 0;
	virtual TREE Sequence() = 0;
	virtual TREE Decorator() = 0;

};