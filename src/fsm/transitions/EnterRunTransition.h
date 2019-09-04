#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"

class FreeModeOnlyCameraState;

class EnterRunTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterRunTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterRunTransition() = default;

	bool CanPerformTransition() const override;
};
