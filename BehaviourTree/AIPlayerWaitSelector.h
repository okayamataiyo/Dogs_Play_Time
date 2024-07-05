#pragma once
//インクルード
#include "Work.h"

class AIPlayerWaitSelector : public Work
{
public:
	NodeState operator()() override;
	AIPlayerWaitSelector();
private:
};

