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
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	OverWaterComponent* DoClone() const;
};

