#pragma once
#include "IInputComponent.h"

class VerticalInputComponent : public IInputComponent
{
public:
	explicit VerticalInputComponent();
	~VerticalInputComponent();

	void OnInputEvent(const InputEvent* event) override;

	VerticalInputComponent* DoClone() const override;
};

