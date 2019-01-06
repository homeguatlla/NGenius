#pragma once
#include "IComponent.h"
class EnvironmentModificatorComponent : public IComponent
{
public:
	EnvironmentModificatorComponent();
	virtual ~EnvironmentModificatorComponent();

private:
	void DoReadFrom(core::utils::IDeserializer* source);
	void DoWriteTo(core::utils::ISerializer* destination);
	EnvironmentModificatorComponent* DoClone() const;
};

