#pragma once
#include "IComponent.h"
class EnvironmentModificatorComponent : public IComponent
{
public:
	EnvironmentModificatorComponent();
	virtual ~EnvironmentModificatorComponent();

	EnvironmentModificatorComponent* DoClone() const;
};

