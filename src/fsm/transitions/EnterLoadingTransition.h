#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"

class FreeModeOnlyCameraState;

class EnterLoadingTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterLoadingTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterLoadingTransition() = default;

	bool CanPerformTransition() const override;
};
