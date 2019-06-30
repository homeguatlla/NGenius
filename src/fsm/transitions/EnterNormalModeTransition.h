#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"


struct GLFWwindow;

class EnterNormalModeTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>
{
public:
	EnterNormalModeTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterNormalModeTransition() = default;

	bool CanPerformTransition() const override;
	void OnInit() override;

private:
	void UpdateInput(GLFWwindow* window);

private:
private:
	std::shared_ptr<NGenius> mNGenius;
	bool mIsNormalModeActive;
};