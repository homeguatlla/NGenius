#pragma once
#include "IComponent.h"
#include<string>

class IGameEntity;

class EnvironmentModificatorComponent : public IComponent
{
public:
	EnvironmentModificatorComponent();
	virtual ~EnvironmentModificatorComponent();

	static std::string GetClassName() { return std::string("EnvironmentModificatorComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	EnvironmentModificatorComponent* DoClone() const;
};

