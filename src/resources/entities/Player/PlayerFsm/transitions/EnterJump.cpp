#include "stdafx.h"
#include "EnterJump.h"
#include "src/resources/entities/Player/Player.h"

EnterJump::EnterJump(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<PlayerState, PlayerContext>(origin, destination)
{
}

void EnterJump::OnInit()
{
	mPlayer = GetContext()->GetPlayer();
}

bool EnterJump::CanPerformTransition() const
{
	return mPlayer->IsActionActive(PlayerAction::Jump);
}