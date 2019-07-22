#pragma once
#include "IComponent.h"
#include<string>

class IGameEntity;

class SpacePartitionComponent :	public IComponent
{
	bool mIsVisible;

public:
	SpacePartitionComponent();
	~SpacePartitionComponent();

	//return true if GameEntity is inside the visible volume of the space partition
	bool IsVisible() const;
	void SetVisibility(bool visibility);

	static std::string GetClassName() { return std::string("SpacePartitionComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	SpacePartitionComponent* DoClone() const override;
};

