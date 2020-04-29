#include "stdafx.h"
#include "EnterWalk.h"
#include "src/resources/components/CollisionComponent.h"
#include "src/resources/entities/Player/Player.h"

#include <iostream>

EnterWalk::EnterWalk(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<PlayerState, PlayerContext>(origin, destination)
{
}

void EnterWalk::OnInit()
{
	mCollisionComponent = GetContext()->GetCollisionComponent();
	mPlayer = GetContext()->GetPlayer();
}

bool EnterWalk::CanPerformTransition() const
{
	return (mPlayer->IsActionActive(PlayerAction::Forward) || mPlayer->IsActionActive(PlayerAction::Backward)) && 
		!mPlayer->IsActionActive(PlayerAction::Run) &&
		mCollisionComponent->IsOnGround();
}