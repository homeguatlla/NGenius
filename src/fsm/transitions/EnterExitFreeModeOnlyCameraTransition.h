#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"

class RunState;

class EnterExitFreeModeOnlyCameraTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterExitFreeModeOnlyCameraTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterExitFreeModeOnlyCameraTransition() = default;

	bool CanPerformTransition() const override;
};
