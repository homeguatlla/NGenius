#pragma once
#include "IInputComponent.h"

class DebugInputComponent :	public IInputComponent
{
public:
	explicit DebugInputComponent(GLFWwindow* window);
	~DebugInputComponent();

	DebugInputComponent* DoClone() const override;
	void Update(float elapsedTime);
};

