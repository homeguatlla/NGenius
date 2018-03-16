#include "stdafx.h"
#include "InputHandler.h"
#include "IInputListener.h"

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
}

void InputHandler::Update(float deltaTime)
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

void InputHandler::OnKey(int key, int action)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnKey(key, action);
	}
}

void InputHandler::OnMouseScroll(int button, float scroll)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnMouseScroll(button, scroll);
	}
}