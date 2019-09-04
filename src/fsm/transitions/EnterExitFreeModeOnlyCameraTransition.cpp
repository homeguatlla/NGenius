#include "stdafx.h"
#include "EnterExitFreeModeOnlyCameraTransition.h"
#include "../../NGenius.h"
#include "../states/RunState.h"


EnterExitFreeModeOnlyCameraTransition::EnterExitFreeModeOnlyCameraTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{	
}

bool EnterExitFreeModeOnlyCameraTransition::CanPerformTransition() const
{
	return GetContext()->IsFreeCameraModeEnabled();
}