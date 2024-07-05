#pragma once
//インクルード
#include <vector>

enum class NodeState
{
	READY = 0,
	RUNNING,
	SUCCESS,
	FAILURE,
};

//継承して働きを作る
class Work
{
public:
	virtual NodeState operator()() = 0;
	Work() {};
private:

};

