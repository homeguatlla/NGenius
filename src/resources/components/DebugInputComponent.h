#pragma once
#include "InputComponent.h"

class DebugInputComponent :	public InputComponent
{
public:
	explicit DebugInputComponent();
	~DebugInputComponent();

	DebugInputComponent* DoClone() const override;
};

