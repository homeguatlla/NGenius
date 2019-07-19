#pragma once
#include "IComponent.h"
#include<string>

class OverWaterComponent : public IComponent
{
	float mWaterHeight;

public:
	OverWaterComponent(float waterHeight);
	~OverWaterComponent();

	float GetWaterHeight() const;

	static std::string GetClassName() { return std::string("OverWaterComponent"); }
	static IComponent* Create();

private:
	OverWaterComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	OverWaterComponent* DoClone() const;
};

