#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"

class FreeModeOnlyCameraState;

class EnterStartTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterStartTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterStartTransition() = default;

	bool CanPerformTransition() const override;
};
