#pragma once
#include "IComponent.h"

class SpacePartitionComponent :	public IComponent
{
public:
	SpacePartitionComponent();
	~SpacePartitionComponent();

	SpacePartitionComponent* DoClone() const override;
};

