//インクルード
#include "../Player/AIPlayer.h"
#include "StateManager.h"
#include "AIPlayerState.h"

AIPlayerAttackDirActionState::AIPlayerAttackDirActionState(StateManager* _pStateManager)
	:StateBase(_pStateManager),pAIPlayer_{nullptr}
{
	pAIPlayer_ = (AIPlayer*)(pStateManager_->GetGameObject());
}

void AIPlayerAttackDirActionState::EnterState()
{
	pAIPlayer_->PlayerAttackDirActionStateFunc();
}

void AIPlayerAttackDirActionState::UpdateState()
{
}

void AIPlayerAttackDirActionState::ExitState()
{
}

AIPlayerAttackGoActionState::AIPlayerAttackGoActionState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
}

void AIPlayerAttackGoActionState::EnterState()
{
}

void AIPlayerAttackGoActionState::UpdateState()
{
}

void AIPlayerAttackGoActionState::ExitState()
{
}

AIPlayerWaitState::AIPlayerWaitState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
}

void AIPlayerWaitState::EnterState()
{
}

void AIPlayerWaitState::UpdateState()
{
}

void AIPlayerWaitState::ExitState()
{
}

AIPlayerWalkState::AIPlayerWalkState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
}

void AIPlayerWalkState::EnterState()
{
}

void AIPlayerWalkState::UpdateState()
{
}

void AIPlayerWalkState::ExitState()
{
}

AIPlayerRunState::AIPlayerRunState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
}

void AIPlayerRunState::EnterState()
{
}

void AIPlayerRunState::UpdateState()
{
}

void AIPlayerRunState::ExitState()
{
}

AIPlayerJumpState::AIPlayerJumpState(StateManager* _pStateManager)
	:StateBase(_pStateManager), pAIPlayer_{ nullptr }
{
}

void AIPlayerJumpState::EnterState()
{
}

void AIPlayerJumpState::UpdateState()
{
}

void AIPlayerJumpState::ExitState()
{
}

AIPlayerStunState::AIPlayerStunState(StateManager* _pStateManager)
{
}

void AIPlayerStunState::EnterState()
{
}

void AIPlayerStunState::UpdateState()
{
}

void AIPlayerStunState::ExitState()
{
}
