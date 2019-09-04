#pragma once
#include "IComponent.h"
#include<string>

class IGameEntity;

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
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	CollisionComponent* DoClone() const override;
};

