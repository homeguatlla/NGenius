#pragma once
#include "../../utils/fsm/BaseState.h"
#include "../../input/IInputListener.h"
#include "../FSMContext.h"
#include "StateTypes.h"

class FreeModeState : public core::utils::FSM::BaseState<NGeniusState, FSMContext>, public IInputListener
{
public:
	FreeModeState();
	virtual ~FreeModeState();

	NGeniusState GetID() const override { return NGeniusState::STATE_FREE;  }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnUpdate(float deltaTime) override;

	bool IsFreeModeActive() const { return mIsFreeModeActivated; }
private:
	void CreateFreeCamera();

private:
	std::shared_ptr<NGenius> mEngine;
	bool mIsFreeModeActivated;

	// Heredado vía IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseCursorPos(double x, double y) override;
};

