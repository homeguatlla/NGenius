#pragma once
#include "IComponent.h"
#include<string>

class IGameEntity;

class OverWaterComponent : public IComponent
{
	float mWaterHeight;

public:
	OverWaterComponent(float waterHeight);
	~OverWaterComponent();

	float GetWaterHeight() const;

	static std::string GetClassName() { return std::string("OverWaterComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	OverWaterComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	OverWaterComponent* DoClone() const;
};

