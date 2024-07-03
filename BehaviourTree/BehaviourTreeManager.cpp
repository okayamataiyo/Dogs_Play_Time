#include "BehaviourTreeManager.h"

//コンストラクタ
BehaviourTreeManager::BehaviourTreeManager(GameObject* _pParent)
	:pParent_(_pParent),currentBefaviourTree_(nullptr)
{
}

//デストラクタ
BehaviourTreeManager::~BehaviourTreeManager()
{
	//プログラム終了時、状態をクリーンアップする
	for (const auto& statePair : BehaviourTreesMap_)
	{
		statePair.second->ExitState();
		delete statePair.second;
	}
}

void BehaviourTreeManager::Update()
{
	if (currentBefaviourTree_ != nullptr)
	{
		currentBefaviourTree_->UpdateState();
	}
}

//状態の変更
void BehaviourTreeManager::ChangeState(const std::string& _behaviourTreeName)
{
	auto it = BehaviourTreesMap_.find(_behaviourTreeName);

	if (it != BehaviourTreesMap_.end())
	{
		if (currentBefaviourTree_ != nullptr)
		{
			currentBefaviourTree_->ExitState();
		}

		currentBefaviourTree_ = it->second;
		currentBefaviourTree_->EnterState();
	}
}

//新しい状態を追加
BehaviourTreeBase* BehaviourTreeManager::AddState(const std::string& _behaviourTreeName, BehaviourTreeBase* _behaviourTree)
{
	auto it = BehaviourTreesMap_.find(_behaviourTreeName);

	if (it != BehaviourTreesMap_.end())
	{
		//既に同じ名前の状態が存在する場合はそのポインタを返す
		return it->second;
	}

	//存在しない場合は新しい状態を登録
	BehaviourTreesMap_[_behaviourTreeName] = _behaviourTree;
	return _behaviourTree;
}
