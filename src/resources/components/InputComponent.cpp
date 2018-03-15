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

const GameEvent* InputComponent::Convert(int key, int action) const
{
	for (const InputConverter* converter : mConverters)
	{
		const GameEvent* event = converter->Convert(key, action);
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