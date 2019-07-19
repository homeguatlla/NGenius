#pragma once
#include "IComponent.h"
#include<string>


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

	static std::string GetClassName() { return std::string("CollisionComponent"); }
	static IComponent* Create();

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	CollisionComponent* DoClone() const override;
};

