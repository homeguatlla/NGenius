#include "stdafx.h"
#include "IInputComponent.h"
#include <GLFW/glfw3.h>

IInputComponent::IInputComponent(GLFWwindow* window) : mWindow(window)
{
}


IInputComponent::~IInputComponent()
{
}
