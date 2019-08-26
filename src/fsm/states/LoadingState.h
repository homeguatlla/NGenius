#pragma once
#include "../../utils/fsm/BaseState.h"
#include "../FSMContext.h"
#include "StateTypes.h"

class LoadingState : public core::utils::FSM::BaseState<NGeniusState, FSMContext>
{
public:
	LoadingState() = default;
	virtual ~LoadingState() = default;
	NGeniusState GetID() const override { return NGeniusState::STATE_LOADING; }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

private:
	std::shared_ptr<NGenius> mEngine;
};

