#include "stdafx.h"
#include "InputComponent.h"
#include "../GameEvent.h"
#include "../../input/InputConverter.h"

InputComponent::InputComponent()
{
}

InputComponent::~InputComponent()
{
}

InputComponent* InputComponent::DoClone() const
{
	return new InputComponent(*this);
}

GameEvent* InputComponent::ConvertKey(int key, int action) const
{
	for (const InputConverter* converter : mConverters)
	{
		GameEvent* event = converter->Convert(key, action);
		if (event != nullptr)
		{
			return event;
		}
	}

	return nullptr;
}

GameEvent* InputComponent::ConvertMouse(void* data) const
{
	for (const InputConverter* converter : mConverters)
	{
		GameEvent* event = converter->Convert(data);
		if (event != nullptr)
		{
			return event;
		}
	}

	return nullptr;
}

void InputComponent::AddConverter(const InputConverter* converter)
{
	bool found = std::find(mConverters.begin(), mConverters.end(), converter) != mConverters.end();
	if (!found)
	{
		mConverters.push_back(converter);
	}
}

void InputComponent::RemoveConverter(const InputConverter* converter)
{
	InputConvertersIterator it = std::find(mConverters.begin(), mConverters.end(), converter);
	bool found = it != mConverters.end();
	if (found)
	{
		mConverters.erase(it);
	}
}