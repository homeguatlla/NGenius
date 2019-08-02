#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"

class FreeModeOnlyCameraState;

class EnterExitFreeModeOnlyPlayerTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterExitFreeModeOnlyPlayerTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterExitFreeModeOnlyPlayerTransition() = default;

	bool CanPerformTransition() const override;
};
