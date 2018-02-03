#pragma once
#include "IComponent.h"

class OverWaterComponent : public IComponent
{
	float mWaterHeight;

public:
	OverWaterComponent(float waterHeight);
	~OverWaterComponent();

	OverWaterComponent* DoClone() const;

	float GetWaterHeight() const;
};

