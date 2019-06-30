#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"

class NormalModeState;

class EnterNormalModeTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterNormalModeTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterNormalModeTransition() = default;

	bool CanPerformTransition() const override;
	void OnInit() override;

private:
private:
	std::shared_ptr<NGenius> mEngine;
	std::shared_ptr<NormalModeState> mOriginState;
};