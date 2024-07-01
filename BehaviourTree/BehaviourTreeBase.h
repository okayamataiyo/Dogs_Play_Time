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

//AI�̃A���S���Y����1��
class BehaviourTreeBase
{
protected:

public:
	BehaviourTreeBase()

	{

	}

	//�f�X�g���N�^
	virtual ~BehaviourTreeBase() {};
	virtual TREE Action() = 0;
	virtual TREE Selector() = 0;
	virtual TREE Sequence() = 0;
	virtual TREE Decorator() = 0;

};