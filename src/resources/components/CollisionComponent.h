#pragma once
#include "IComponent.h"


class CollisionComponent : public IComponent
{
	bool mIsOnGround;
	float mGroundHeight;

public:
	explicit CollisionComponent();
	~CollisionComponent();

	bool IsOnGround() const;
	void SetOnGround(bool isOnGround);
	void SetGroundHeight(float groundHeight);
	float GetGroundHeight() const;

private:
	CollisionComponent* DoClone() const override;
};

