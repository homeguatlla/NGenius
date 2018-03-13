#include "stdafx.h"
#include "IInputComponent.h"
#include "../../input/InputEvent.h"
#include "../../input/InputConverter.h"
#include <GLFW/glfw3.h>

IInputComponent::IInputComponent()
{
}

IInputComponent::~IInputComponent()
{
}

void IInputComponent::OnKey(int key, int action)
{
	for (const InputConverter* converter : mConverters)
	{
		const InputEvent* event = converter->Convert(key, action);
		if (event != nullptr)
		{
			mEvents.push(event);
			break;
		}
	}
}

void IInputComponent::OnMouseScroll(float scroll)
{
	
}

void IInputComponent::Update(float elapsedTime)
{
	while (!mEvents.empty())
	{
		OnInputEvent(mEvents.front());
		mEvents.pop();
	}
}

void IInputComponent::AddConverter(const InputConverter* converter)
{
	bool found = std::find(mConverters.begin(), mConverters.end(), converter) != mConverters.end();
	if (!found)
	{
		mConverters.push_back(converter);
	}
}

void IInputComponent::RemoveConverter(const InputConverter* converter)
{
	InputConvertersIterator it = std::find(mConverters.begin(), mConverters.end(), converter);
	bool found = it != mConverters.end();
	if (found)
	{
		mConverters.erase(it);
	}
}
