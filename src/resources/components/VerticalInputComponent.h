#pragma once
#include "InputComponent.h"

class VerticalInputComponent : public InputComponent
{
public:
	explicit VerticalInputComponent();
	~VerticalInputComponent();

	VerticalInputComponent* DoClone() const override;
};

