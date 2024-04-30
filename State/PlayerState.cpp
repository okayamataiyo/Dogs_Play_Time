//インクルード
#include "../Player/CollectPlayer.h"
#include "../Player/AttackPlayer.h"
#include "StateManager.h"
#include "PlayerState.h"

PlayerWaitState::PlayerWaitState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerWaitState::EnterState()
{
	pAttackPlayer_->PlayerWaitStateFunc();
}

void PlayerWaitState::UpdateState()
{
	if (pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump() && !pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}

	if (pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("JumpState");
	}

	if (pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void PlayerWaitState::ExitState()
{
}

PlayerWalkState::PlayerWalkState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerWalkState::EnterState()
{

	pAttackPlayer_->PlayerWalkStateFunc();
}

void PlayerWalkState::UpdateState()
{
	if (!pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}

	if (pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("JumpState");
	}

	if (pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void PlayerWalkState::ExitState()
{
}

PlayerRunState::PlayerRunState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerRunState::EnterState()
{
	pAttackPlayer_->PlayerRunStateFunc();
}

void PlayerRunState::UpdateState()
{
	if (!pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump() && !pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("JumpState");
	}

	if (pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void PlayerRunState::ExitState()
{
}

PlayerJumpState::PlayerJumpState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerJumpState::EnterState()
{
	pAttackPlayer_->PlayerJumpStateFunc();
}

void PlayerJumpState::UpdateState()
{
	if (!pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump() && !pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}

	if (pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("StunState");
	}
}

void PlayerJumpState::ExitState()
{

}

PlayerStunState::PlayerStunState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr}, pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerStunState::EnterState()
{
	pAttackPlayer_->PlayerStunStateFunc();
}

void PlayerStunState::UpdateState()
{
	if (!pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump() && !pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAttackPlayer_->GetIsRun())
	{
		pStateManager_->ChangeState("RunState");
	}
}

void PlayerStunState::ExitState()
{
}
