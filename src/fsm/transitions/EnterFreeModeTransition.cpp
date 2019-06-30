#include "stdafx.h"
#include "EnterFreeModeTransition.h"
#include "../../NGenius.h"
#include "../states/FreeModeState.h"


EnterFreeModeTransition::EnterFreeModeTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{
	mEngine = GetContext()->GetEngine();	
	mOriginState = std::static_pointer_cast<FreeModeState>(origin);
}

void EnterFreeModeTransition::OnInit()
{

}

bool EnterFreeModeTransition::CanPerformTransition() const
{
	return mOriginState->IsFreeModeActive();
}