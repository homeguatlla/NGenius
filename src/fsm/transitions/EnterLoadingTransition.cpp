#include "stdafx.h"
#include "EnterLoadingTransition.h"
#include "../../NGenius.h"
#include "../states/FreeModeOnlyCameraState.h"


EnterLoadingTransition::EnterLoadingTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{
}

bool EnterLoadingTransition::CanPerformTransition() const
{
	return GetContext()->GetNextState() == NGeniusState::STATE_LOADING;
}