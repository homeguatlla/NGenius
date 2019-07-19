#pragma once
#include "IComponent.h"
#include<string>

class EnvironmentModificatorComponent : public IComponent
{
public:
	EnvironmentModificatorComponent();
	virtual ~EnvironmentModificatorComponent();

	static std::string GetClassName() { return std::string("EnvironmentModificatorComponent"); }
	static IComponent* Create();

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	EnvironmentModificatorComponent* DoClone() const;
};

