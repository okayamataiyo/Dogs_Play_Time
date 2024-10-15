#pragma once
class StateManager;

//状態ベースクラス
class StateBase
{
protected:
	StateManager* pStateManager_;
public:
	StateBase(StateManager* _manager)
		:pStateManager_(_manager)
	{

	}

	//デストラクタ
	virtual ~StateBase() {};
	//状態に入る時の初期化
	virtual void EnterState() = 0;
	//状態の更新
	virtual void UpdateState() = 0;
	//状態から出る時の後処理
	virtual void ExitState() = 0;
};