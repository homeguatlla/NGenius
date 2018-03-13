#pragma once
#include "IInputComponent.h"

class DebugInputComponent :	public IInputComponent
{
public:
	explicit DebugInputComponent();
	~DebugInputComponent();

	void OnInputEvent(const InputEvent* event) override;
	DebugInputComponent* DoClone() const override;
};

