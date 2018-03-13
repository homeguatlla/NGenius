#pragma once
#include "IInputComponent.h"

class DebugInputComponent :	public IInputComponent
{
public:
	explicit DebugInputComponent();
	~DebugInputComponent();

	DebugInputComponent* DoClone() const override;
};

