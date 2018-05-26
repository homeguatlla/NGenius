#pragma once
#include "IComponent.h"

class SpacePartitionComponent :	public IComponent
{
	bool mIsVisible;

public:
	SpacePartitionComponent();
	~SpacePartitionComponent();

	//return true if GameEntity is inside the visible volume of the space partition
	bool IsVisible() const;
	void SetVisibility(bool visibility);
	
	SpacePartitionComponent* DoClone() const override;
};

