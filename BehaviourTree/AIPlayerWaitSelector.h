#pragma once
//�C���N���[�h
#include "Work.h"

class AIPlayerWaitSelector : public Work
{
public:
	NodeState operator()() override;
	AIPlayerWaitSelector();
private:
};

