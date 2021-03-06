#pragma once
#include "src/utils/fsm/BaseState.h"
#include "src/input/IInputListener.h"
#include "src/fsm/FSMContext.h"
#include "StateTypes.h"

#include <glm/glm.hpp>

class ICamera;
class Text;
class IGameEntity;

class FreeModeOnlyCameraState : public core::utils::FSM::BaseState<NGeniusState, FSMContext>, public IInputListener
{
public:
	FreeModeOnlyCameraState();
	virtual ~FreeModeOnlyCameraState();

	NGeniusState GetID() const override { return NGeniusState::STATE_FREE_CAMERA;  }
	void OnInit() override;
	void OnEnter(float deltaTime) override;
	void OnExit(float deltaTime) override;
	void OnReload() override;
	void OnUpdate(float deltaTime) override;

private:
	ICamera* CreateFreeCamera();
	void CalculateCameraOrientation(float deltaTime);
	void CalculateCameraPosition(float deltaTime);	

	// Heredado v�a IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseCursorPos(double x, double y) override;

	void SaveFreeCameraData();
	void RecoverFreeCameraData();

private:
	std::shared_ptr<NGenius> mEngine;
	ICamera* mFreeCamera;
	glm::vec3 mSavedCameraPosition;
	glm::vec3 mSavedCameraTarget;
	float mPitch;
	float mLastPitch;
	float mRoll;
	float mLastRoll;
	float mForwardSpeed;
	float mPanSpeed;

protected:
	std::shared_ptr<IGameEntity> mPlayer;
};

