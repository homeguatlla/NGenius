#pragma once
#include "../../utils/fsm/BaseTransition.h"
#include "../../fsm/states/StateTypes.h"
#include "../../fsm/FSMContext.h"
#include "../../input/IInputListener.h"


class EnterNormalModeTransition : public core::utils::FSM::BaseTransition<NGeniusState, FSMContext>, public IInputListener
{
public:
	EnterNormalModeTransition(StatePtr origin, StatePtr destination);
	virtual ~EnterNormalModeTransition();

	bool CanPerformTransition() const override;
	void OnInit() override;

private:
private:
	std::shared_ptr<NGenius> mEngine;
	bool mIsNormalModeActive;

	// Heredado v�a IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override {}
	void OnMouseButton(int button, int action, int mods) override {}
	void OnMouseCursorPos(double x, double y) override {}
};