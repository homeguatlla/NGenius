#pragma once
#include "IInputComponent.h"
class PlayerInputComponent : public IInputComponent
{
	const float mRunSpeed;
	const float mTurnSpeed;
	const float mUpwardsSpeed;

	float mCurrentRunSpeed;
	float mCurrentTurnSpeed;
	float mCurrentUpwardsSpeed;

	bool mHasMoved;
	bool mHasJumped;

public:
	explicit PlayerInputComponent(GLFWwindow* window, float runSpeed, float turnSpeed, float upwardsSpeed);
	virtual ~PlayerInputComponent();

	PlayerInputComponent* DoClone() const override;

	void Update(float elapsedTime);

	float GetCurrentRunSpeed() const;
	float GetCurrentTurnSpeed() const;
	float GetCurrentUpwardsSpeed() const;
	bool HasMoved() const;
	bool HasJumped() const;

private:
	bool UpdateTurnSpeed(float elapsedTime);
	bool UpdateRunSpeed(float elapsedTime);
	bool UpdateUpwardsSpeed(float elapsedTime);
};

