#pragma once
#include "IInputComponent.h"

class VerticalInputComponent : public IInputComponent
{
public:
	explicit VerticalInputComponent(GLFWwindow* window);
	~VerticalInputComponent();

	VerticalInputComponent* DoClone() const override;

	void Update(float elapsedTime);
};

