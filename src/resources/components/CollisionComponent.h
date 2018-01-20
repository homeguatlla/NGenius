#pragma once
#include "IComponent.h"


class CollisionComponent : public IComponent
{
	bool mIsOnGround;
	float mGroundHeight;

public:
	explicit CollisionComponent();
	~CollisionComponent();

	CollisionComponent* DoClone() const override;
	bool IsOnGround() const;
	void SetOnGround(bool isOnGround);
	void SetGroundHeight(float groundHeight);
	float GetGroundHeight() const;
};

