#include "stdafx.h"
#include "EnterStartTransition.h"
#include "../../NGenius.h"


EnterStartTransition::EnterStartTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{
}

bool EnterStartTransition::CanPerformTransition() const
{
	return GetContext()->GetNextState() == NGeniusState::STATE_START;
}