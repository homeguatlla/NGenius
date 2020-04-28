#include "stdafx.h"
#include "EnterIdle.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/CollisionComponent.h"
#include "source/utils/Math.h"

EnterIdle::EnterIdle(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<PlayerState, PlayerContext>(origin, destination)
{
}

void EnterIdle::OnInit()
{
	mPhysicsComponent = GetContext()->GetPhysicsComponent();
	mCollisionComponent = GetContext()->GetCollisionComponent();
}

bool EnterIdle::CanPerformTransition() const
{
	return NPhysics::NMath::IsNearlyEqual(mPhysicsComponent->GetVelocity(), glm::vec3(0.0f)) &&
		mCollisionComponent->IsOnGround();
}