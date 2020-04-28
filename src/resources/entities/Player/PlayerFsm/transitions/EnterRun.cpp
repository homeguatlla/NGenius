#include "stdafx.h"
#include "EnterRun.h"
#include "src/resources/components/CollisionComponent.h"
#include "src/resources/entities/Player/Player.h"

#include <iostream>

EnterRun::EnterRun(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<PlayerState, PlayerContext>(origin, destination)
{
}

void EnterRun::OnInit()
{
	mCollisionComponent = GetContext()->GetCollisionComponent();
	mPlayer = GetContext()->GetPlayer();
}

bool EnterRun::CanPerformTransition() const
{
	return (mPlayer->IsActionActive(PlayerAction::Forward) || mPlayer->IsActionActive(PlayerAction::Backward)) && 
		mCollisionComponent->IsOnGround();
}