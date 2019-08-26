#pragma once
#include "../../utils/fsm/BaseState.h"
#include "../FSMContext.h"
#include "StateTypes.h"

class StartState : public core::utils::FSM::BaseState<NGeniusState, FSMContext>
{
public:
	StartState() = default;
	virtual ~StartState() = default;
	NGeniusState GetID() const override { return NGeniusState::STATE_START; }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

private:
	std::shared_ptr<NGenius> mEngine;
};

