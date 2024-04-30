//インクルード
#include "../Player/CollectPlayer.h"
#include "../Player/AttackPlayer.h"
#include "StateManager.h"
#include "AttackPlayerState.h"

AttackPlayerWaitState::AttackPlayerWaitState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void AttackPlayerWaitState::EnterState()
{
	pAttackPlayer_->PlayerWaitStateFunc();
}

void AttackPlayerWaitState::UpdateState()
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

void AttackPlayerWaitState::ExitState()
{
}

AttackPlayerWalkState::AttackPlayerWalkState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void AttackPlayerWalkState::EnterState()
{

	pAttackPlayer_->PlayerWalkStateFunc();
}

void AttackPlayerWalkState::UpdateState()
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

void AttackPlayerWalkState::ExitState()
{
}

AttackPlayerRunState::AttackPlayerRunState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void AttackPlayerRunState::EnterState()
{
	pAttackPlayer_->PlayerRunStateFunc();
}

void AttackPlayerRunState::UpdateState()
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

void AttackPlayerRunState::ExitState()
{
}

AttackPlayerJumpState::AttackPlayerJumpState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void AttackPlayerJumpState::EnterState()
{
	pAttackPlayer_->PlayerJumpStateFunc();
}

void AttackPlayerJumpState::UpdateState()
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

void AttackPlayerJumpState::ExitState()
{

}

AttackPlayerStunState::AttackPlayerStunState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr}, pAttackPlayer_{nullptr}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void AttackPlayerStunState::EnterState()
{
	pAttackPlayer_->PlayerStunStateFunc();
}

void AttackPlayerStunState::UpdateState()
{
	if (!pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump() && !pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("WaitState");
	}

	if (pAttackPlayer_->GetIsMove() && !pAttackPlayer_->GetIsJump() && !pAttackPlayer_->GetIsRun() && !pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("WalkState");
	}

	if (pAttackPlayer_->GetIsRun() && !pAttackPlayer_->GetIsStun())
	{
		pStateManager_->ChangeState("RunState");
	}
}

void AttackPlayerStunState::ExitState()
{
}
