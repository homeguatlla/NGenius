#pragma once
#include "../../resources/components/IComponent.h"

class PickedUpComponent : public IComponent
{
public:
	PickedUpComponent();
	virtual ~PickedUpComponent();

	// Heredado v�a IComponent
	IComponent * DoClone() const override;
};

