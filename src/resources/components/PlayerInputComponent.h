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
	explicit PlayerInputComponent(float runSpeed, float turnSpeed, float upwardsSpeed);
	virtual ~PlayerInputComponent();

	PlayerInputComponent* DoClone() const override;

	void Update(float elapsedTime) {};

	float GetCurrentRunSpeed() const;
	float GetCurrentTurnSpeed() const;
	float GetCurrentUpwardsSpeed() const;

	bool HasMoved() const;
	bool HasJumped() const;

private:
	void OnKey(int key, int action) override;
	void OnMouseScroll(float scroll) override {}
	void UpdateTurnSpeed(int key, int action);
	void UpdateRunSpeed(int key, int action);
	void UpdateUpwardsSpeed(int action);
};

