#include "stdafx.h"
#include "InputManager.h"
#include "IInputListener.h"

InputManager::InputManager() : mWindow(nullptr), mIsInitialized(false)
{
}


InputManager::~InputManager()
{
	mListeners.clear();
	mInputHandler = nullptr;
}

void InputManager::Init(GLFWwindow* window)
{
	mWindow = window;
	mIsInitialized = true;
}

void InputManager::Update(float deltaTime)
{
	if (mIsInitialized)
	{
		mInputHandler(mWindow);
		/*
		if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
		{
			if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}*/
	}
}

void InputManager::RegisterAllEventsInputListener(IInputListener* listener)
{
	bool found = std::find(mListeners.begin(), mListeners.end(), listener) != mListeners.end();
	if (!found)
	{
		mListeners.push_back(listener);
	}
}

void InputManager::UnRegisterInputListener(IInputListener* listener)
{
	ListenersIterator it = std::find(mListeners.begin(), mListeners.end(), listener);
	bool found = it != mListeners.end();
	if (found)
	{
		mListeners.erase(it);
	}
}

void InputManager::RegisterInputHandler(std::function<void(GLFWwindow* window)> callback)
{
	mInputHandler = callback;
}

void InputManager::OnKey(int key, int action)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnKey(key, action);
	}
}

void InputManager::OnMouseScroll(float scroll)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnMouseScroll(scroll);
	}
}