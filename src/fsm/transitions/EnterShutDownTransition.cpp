#include "stdafx.h"
#include "EnterShutDownTransition.h"
#include "../../NGenius.h"
#include "../states/FreeModeOnlyCameraState.h"


EnterShutDownTransition::EnterShutDownTransition(StatePtr origin) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin)
{
}

bool EnterShutDownTransition::CanPerformTransition() const
{
	return GetContext()->GetNextState() == NGeniusState::STATE_SHUT_DOWN;
}