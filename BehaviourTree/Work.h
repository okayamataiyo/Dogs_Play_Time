#pragma once
//�C���N���[�h
#include <vector>

enum class NodeState
{
	READY = 0,
	RUNNING,
	SUCCESS,
	FAILURE,
};

//�p�����ē��������
class Work
{
public:
	virtual NodeState operator()() = 0;
	Work() {};
private:

};

