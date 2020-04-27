#include "stdafx.h"
#include "EnterRun.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/CharacterComponent.h"
#include "src/resources/events/characterControllerEvents/BackwardEvent.h"
#include "src/resources/events/characterControllerEvents/ForwardEvent.h"
#include "source/utils/Math.h"

EnterRun::EnterRun(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<PlayerState, PlayerContext>(origin, destination)
{
}

void EnterRun::OnInit()
{
	mPhysicsComponent = GetContext()->GetPhysicsComponent();
	mCharacterComponent = GetContext()->GetCharacterComponent();
}

bool EnterRun::CanPerformTransition() const
{
	return mCharacterComponent->IsNextEventOfType<ForwardEvent>() || 
		mCharacterComponent->IsNextEventOfType<BackwardEvent>();
}