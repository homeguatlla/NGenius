#pragma once
#include "IComponent.h"

struct GLFWwindow;

class IInputComponent : public IComponent
{
protected:
	GLFWwindow* mWindow;

public:
	explicit IInputComponent(GLFWwindow* window);
	virtual ~IInputComponent();

	virtual void Update(float elapsedTime) = 0;
};

