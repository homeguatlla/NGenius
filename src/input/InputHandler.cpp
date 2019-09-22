#include "stdafx.h"
#include "InputHandler.h"
#include "IInputListener.h"

#include <GLFW/glfw3.h>
#include <iostream>

InputHandler::InputHandler() : mWindow(nullptr), mIsInitialized(false)
{
}

InputHandler::~InputHandler()
{
	mListeners.clear();
	mInputHandler = nullptr;
}

void InputHandler::Init(GLFWwindow* window)
{
	mWindow = window;
	mIsInitialized = true;
	glfwSetWindowUserPointer(window, this);
	glfwSetScrollCallback(mWindow, &InputHandler::OnMouseScroll);
	glfwSetKeyCallback(mWindow, &InputHandler::OnKeyCallback);
	glfwSetMouseButtonCallback(mWindow, &InputHandler::OnMouseButtonCallback);
	glfwSetCursorPosCallback(mWindow, &InputHandler::OnMouseCursorPosCallback);
}

void InputHandler::Update(float deltaTime)
{
	if (mIsInitialized && mInputHandler != nullptr)
	{
		mInputHandler(mWindow);
	}
}

void InputHandler::RegisterAllEventsInputListener(IInputListener* listener)
{
	bool found = std::find(mListeners.begin(), mListeners.end(), listener) != mListeners.end();
	if (!found)
	{
		mListeners.push_back(listener);
	}
}

void InputHandler::UnRegisterInputListener(IInputListener* listener)
{
	ListenersIterator it = std::find(mListeners.begin(), mListeners.end(), listener);
	bool found = it != mListeners.end();
	if (found)
	{
		mListeners.erase(it);
	}
}

void InputHandler::RegisterInputHandler(std::function<void(GLFWwindow* window)> callback)
{
	mInputHandler = callback;
}

void InputHandler::OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	handler->MouseScroll(GLFW_MOUSE_BUTTON_MIDDLE, static_cast<float>(yOffset));
}

void InputHandler::OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	handler->Key(key, action);
	//std::cout << "key = " << key << " action = " << action << "\n";
}

void InputHandler::OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	handler->MouseButton(button, action, mods);
}

void InputHandler::OnMouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputHandler* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
	handler->MouseCursorPos(xpos, ypos);
	//std::cout << "cursor X = " << xpos  << "\n";
}

void InputHandler::Key(int key, int action)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnKey(key, action);
	}
}

void InputHandler::MouseScroll(int button, float scroll)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnMouseScroll(button, scroll);
	}
}

void InputHandler::MouseButton(int button, int action, int mods)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnMouseButton(button, action, mods);
	}
}

void InputHandler::MouseCursorPos(double x, double y)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnMouseCursorPos(x, y);
	}
}