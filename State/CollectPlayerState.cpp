//インクルード
#include "../Player/CollectPlayer.h"
#include "../Player/AttackPlayer.h"
#include "StateManager.h"
#include "CollectPlayerState.h"

CollectPlayerWaitState::CollectPlayerWaitState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void CollectPlayerWaitState::EnterState()
{
	pAttackPlayer_->PlayerWaitStateFunc();
}

void CollectPlayerWaitState::UpdateState()
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

void CollectPlayerWaitState::ExitState()
{
}

CollectPlayerWalkState::CollectPlayerWalkState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void CollectPlayerWalkState::EnterState()
{

	pAttackPlayer_->PlayerWalkStateFunc();
}

void CollectPlayerWalkState::UpdateState()
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

void CollectPlayerWalkState::ExitState()
{
}

CollectPlayerRunState::CollectPlayerRunState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void CollectPlayerRunState::EnterState()
{
	pAttackPlayer_->PlayerRunStateFunc();
}

void CollectPlayerRunState::UpdateState()
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

void CollectPlayerRunState::ExitState()
{
}

CollectPlayerJumpState::CollectPlayerJumpState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void CollectPlayerJumpState::EnterState()
{
	pAttackPlayer_->PlayerJumpStateFunc();
}

void CollectPlayerJumpState::UpdateState()
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

void CollectPlayerJumpState::ExitState()
{

}

CollectPlayerStunState::CollectPlayerStunState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void CollectPlayerStunState::EnterState()
{
	pAttackPlayer_->PlayerStunStateFunc();
}

void CollectPlayerStunState::UpdateState()
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

void CollectPlayerStunState::ExitState()
{
}
