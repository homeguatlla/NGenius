#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"


struct GLFWwindow;

class EnterFreeModeTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterFreeModeTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterFreeModeTransition() = default;

	bool CanPerformTransition() const override;
	void OnInit() override;

private:
	void UpdateInput(GLFWwindow* window);

private:
	std::shared_ptr<NGenius> mNGenius;
	bool mIsFreeModeActive;
};
