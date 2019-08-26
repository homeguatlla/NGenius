#include "stdafx.h"
#include "EnterRunTransition.h"
#include "../../NGenius.h"


EnterRunTransition::EnterRunTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{
}

bool EnterRunTransition::CanPerformTransition() const
{
	return GetContext()->GetNextState() == NGeniusState::STATE_RUN;
}