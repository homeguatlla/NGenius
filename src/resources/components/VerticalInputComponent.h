#pragma once
#include "IInputComponent.h"

class VerticalInputComponent : public IInputComponent
{
public:
	explicit VerticalInputComponent();
	~VerticalInputComponent();

	VerticalInputComponent* DoClone() const override;
};

