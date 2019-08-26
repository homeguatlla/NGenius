#pragma once
#include "../../utils/fsm/BaseState.h"
#include "../../input/IInputListener.h"
#include "../FSMContext.h"
#include "StateTypes.h"

class RunState : public core::utils::FSM::BaseState<NGeniusState, FSMContext>, public IInputListener
{
public:
	RunState();
	virtual ~RunState();
	NGeniusState GetID() const override { return NGeniusState::STATE_RUN; }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

private:
	// Heredado vía IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override {}
	void OnMouseButton(int button, int action, int mods) override {}
	void OnMouseCursorPos(double x, double y) override {}

private:
	std::shared_ptr<NGenius> mEngine;
};

