//インクルード
#include "../Player/AIPlayer.h"
#include "StateManager.h"
#include "AIPlayerState.h"

AIPlayerWaitState::AIPlayerWaitState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
	pAIPlayer_ = (AIPlayer*)(pStateManager_->GetGameObject());
}

void AIPlayerWaitState::EnterState()
{
	pAIPlayer_->PlayerWaitStateFunc();
}

void AIPlayerWaitState::UpdateState()
{
	if (pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump() && !pAIPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAIPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}

	if (pAIPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("JumpState");
	}

	if (pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void AIPlayerWaitState::ExitState()
{
}

AIPlayerWalkState::AIPlayerWalkState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
	pAIPlayer_ = (AIPlayer*)(pStateManager_->GetGameObject());
}

void AIPlayerWalkState::EnterState()
{
	pAIPlayer_->PlayerWalkStateFunc();
}

void AIPlayerWalkState::UpdateState()
{
	if (!pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAIPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}

	if (pAIPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("JumpState");
	}

	if (pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void AIPlayerWalkState::ExitState()
{
}

AIPlayerRunState::AIPlayerRunState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
	pAIPlayer_ = (AIPlayer*)(pStateManager_->GetGameObject());
}

void AIPlayerRunState::EnterState()
{
	pAIPlayer_->PlayerRunStateFunc();
}

void AIPlayerRunState::UpdateState()
{
	if (!pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump() && !pAIPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAIPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("JumpState");
	}

	if (pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void AIPlayerRunState::ExitState()
{
}

AIPlayerJumpState::AIPlayerJumpState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
	pAIPlayer_ = (AIPlayer*)(pStateManager_->GetGameObject());
}

void AIPlayerJumpState::EnterState()
{
	pAIPlayer_->PlayerJumpStateFunc();
}

void AIPlayerJumpState::UpdateState()
{
	if (!pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump() && !pAIPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAIPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}

	if (pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void AIPlayerJumpState::ExitState()
{
}

AIPlayerStunState::AIPlayerStunState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
	pAIPlayer_ = (AIPlayer*)(pStateManager_->GetGameObject());
}

void AIPlayerStunState::EnterState()
{
	pAIPlayer_->PlayerStunStateFunc();
}

void AIPlayerStunState::UpdateState()
{
	if (!pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump() && !pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAIPlayer_->GetIsMove() && !pAIPlayer_->GetIsJump() && !pAIPlayer_->GetIsRun() && !pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAIPlayer_->GetIsRun() && !pAIPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("RunState");
	}
}

void AIPlayerStunState::ExitState()
{
}
