//インクルード
#include "../Player/CollectPlayer.h"
#include "../Player/AttackPlayer.h"
#include "StateManager.h"
#include "PlayerState.h"

PlayerWaitState::PlayerWaitState(StateManager* _pStateManager) : StateBase(_pStateManager), pCollectPlayer_{ nullptr }, pAttackPlayer_{ nullptr },isChangeState_{false},isChangeStatePrev_{false}
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerWaitState::EnterState()
{

}

void PlayerWaitState::UpdateState()
{
	if (pCollectPlayer_->IsMoving() && !pCollectPlayer_->GetIsJump() && !pCollectPlayer_->GetIsDash())
	{
		pStateManager_->ChangeState("WalkState");
	}
}

void PlayerWaitState::ExitState()
{
}

PlayerWalkState::PlayerWalkState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}, isChangeState_{ false }, isChangeStatePrev_{ false }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerWalkState::EnterState()
{
}

void PlayerWalkState::UpdateState()
{
	pAttackPlayer_->PlayerWalkStateFunc();
}

void PlayerWalkState::ExitState()
{
}

PlayerRunState::PlayerRunState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}, isChangeState_{ false }, isChangeStatePrev_{ false }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerRunState::EnterState()
{
}

void PlayerRunState::UpdateState()
{
}

void PlayerRunState::ExitState()
{
}

PlayerJumpState::PlayerJumpState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr},pAttackPlayer_{nullptr}, isChangeState_{ false }, isChangeStatePrev_{ false }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerJumpState::EnterState()
{
}

void PlayerJumpState::UpdateState()
{
}

void PlayerJumpState::ExitState()
{
}

PlayerStunState::PlayerStunState(StateManager* _pStateManager) : StateBase(_pStateManager),pCollectPlayer_{nullptr}, pAttackPlayer_{nullptr}, isChangeState_{ false }, isChangeStatePrev_{ false }
{
	pCollectPlayer_ = (CollectPlayer*)(pStateManager_->GetGameObject());
	pAttackPlayer_ = (AttackPlayer*)(pStateManager_->GetGameObject());
}

void PlayerStunState::EnterState()
{
}

void PlayerStunState::UpdateState()
{
}

void PlayerStunState::ExitState()
{
}
