#pragma once
#include "IComponent.h"

class OverWaterComponent : public IComponent
{
	float mWaterHeight;

public:
	OverWaterComponent(float waterHeight);
	~OverWaterComponent();

	float GetWaterHeight() const;

private:
	OverWaterComponent* DoClone() const;
};

