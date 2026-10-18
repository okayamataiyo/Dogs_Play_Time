#include "StateManager.h"

//コンストラクタ
StateManager::StateManager(GameObject* _pParent)
	:pParent_(_pParent), currentState_(nullptr)
{

}

//デストラクタ
StateManager::~StateManager()
{
	//プログラム終了時、状態をクリーンアップする（名前の順に、ExitStateを呼んでから消す）
	for (auto& statePair : statesMap_)
	{
		statePair.second->ExitState();
		statePair.second.reset();
	}
}


void StateManager::Update()
{
	if (currentState_ != nullptr)
	{
		currentState_->UpdateState();
	}
}

//状態の変更
void StateManager::ChangeState(const std::string& _stateName)
{
	auto it = statesMap_.find(_stateName);

	if (it != statesMap_.end())
	{
		if (currentState_ != nullptr)
		{
			currentState_->ExitState();
		}

		currentState_ = it->second.get();
		currentState_->EnterState();
	}
}

//新しい状態を追加
StateBase* StateManager::AddState(const std::string& _stateName, std::unique_ptr<StateBase> _state)
{
	auto it = statesMap_.find(_stateName);

	if (it != statesMap_.end())
	{
		//既に同じ名前の状態が存在する場合は、渡された状態は破棄して、既にあるものを返す
		return it->second.get();
	}

	//存在しない場合は新しい状態を登録
	StateBase* pState = _state.get();
	statesMap_[_stateName] = std::move(_state);
	return pState;
}