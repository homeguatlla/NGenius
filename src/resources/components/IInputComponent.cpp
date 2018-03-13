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


bool IInputComponent::IsInputsEventsEmpty() const
{
	return mEvents.empty();
}

const InputEvent* IInputComponent::ConsumeInputEvent()
{
	const InputEvent* event = mEvents.front();
	mEvents.pop();

	return event;
}