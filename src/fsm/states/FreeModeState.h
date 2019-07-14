#pragma once
#include "../../utils/fsm/BaseState.h"
#include "../../input/IInputListener.h"
#include "../FSMContext.h"
#include "StateTypes.h"

#include <glm/glm.hpp>

class ICamera;
class Text;

class FreeModeState : public core::utils::FSM::BaseState<NGeniusState, FSMContext>, public IInputListener
{
public:
	FreeModeState();
	virtual ~FreeModeState();

	NGeniusState GetID() const override { return NGeniusState::STATE_FREE;  }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnReload() override;
	void OnUpdate(float deltaTime) override;

	bool IsNormalModeKeyPressed() const { return mIsNormalModeKeyPressed; }
private:
	ICamera* CreateFreeCamera();
	void CalculateCameraOrientation(float deltaTime);
	void CalculateCameraPosition(float deltaTime);	

	// Heredado vía IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseCursorPos(double x, double y) override;

	void CreateFreeModeStateUI();

private:
	std::shared_ptr<NGenius> mEngine;
	ICamera* mFreeCamera;
	glm::vec3 mCameraPosition;
	glm::vec3 mCameraDirection;
	bool mIsNormalModeKeyPressed;
	float mPitch;
	float mLastPitch;
	float mRoll;
	float mLastRoll;
	float mForwardSpeed;
	Text* mFreeModeText;
};

