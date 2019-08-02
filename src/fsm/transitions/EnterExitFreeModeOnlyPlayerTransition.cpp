#include "stdafx.h"
#include "EnterExitFreeModeOnlyPlayerTransition.h"
#include "../../NGenius.h"
#include "../states/FreeModeOnlyCameraState.h"


EnterExitFreeModeOnlyPlayerTransition::EnterExitFreeModeOnlyPlayerTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{
}

bool EnterExitFreeModeOnlyPlayerTransition::CanPerformTransition() const
{
	return GetContext()->IsFreePlayerModeEnabled();
}