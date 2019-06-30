#include "stdafx.h"
#include "EnterNormalModeTransition.h"
#include "../../NGenius.h"
#include "../states/FreeModeState.h"

EnterNormalModeTransition::EnterNormalModeTransition(StatePtr origin, StatePtr destination) :
	core::utils::FSM::BaseTransition<NGeniusState, FSMContext>(origin, destination)
{
	mOriginState = std::static_pointer_cast<FreeModeState>(origin);
}

void EnterNormalModeTransition::OnInit()
{
	mEngine = GetContext()->GetEngine();
}

bool EnterNormalModeTransition::CanPerformTransition() const
{
	return mOriginState->IsNormalModeKeyPressed();
}