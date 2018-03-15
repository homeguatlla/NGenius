#pragma once
#include "InputComponent.h"

class PlayerInputComponent : public InputComponent
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
	explicit PlayerInputComponent(float runSpeed, float turnSpeed, float upwardsSpeed);
	virtual ~PlayerInputComponent();

	PlayerInputComponent* DoClone() const override;

	float GetCurrentRunSpeed() const;
	float GetCurrentTurnSpeed() const;
	float GetCurrentUpwardsSpeed() const;

	bool HasMoved() const;
	bool HasJumped() const;

private:
	void OnInputEvent(const GameEvent* event);
	void UpdateTurnSpeed(int key, int action);
	void UpdateRunSpeed(bool isForward);
	void UpdateUpwardsSpeed(int action);
};

