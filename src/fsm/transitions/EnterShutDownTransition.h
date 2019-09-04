#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"


class EnterShutDownTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterShutDownTransition(StatePtr origin);
	virtual ~EnterShutDownTransition() = default;

	bool CanPerformTransition() const override;
};
