#include "stdafx.h"
#include "EnterOnAir.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/CollisionComponent.h"

EnterOnAir::EnterOnAir(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<PlayerState, PlayerContext>(origin, destination)
{
}

void EnterOnAir::OnInit()
{
	mPhysicsComponent = GetContext()->GetPhysicsComponent();
	mCollisionComponent = GetContext()->GetCollisionComponent();
}

bool EnterOnAir::CanPerformTransition() const
{
	bool isHeightHighEnough = mPhysicsComponent->GetPhysicsObject()->GetPosition().y - mCollisionComponent->GetGroundHeight() > 0.5f;
	return !mCollisionComponent->IsOnGround() && isHeightHighEnough;
}