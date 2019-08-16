#include "stdafx.h"
#include "InputComponent.h"
#include "../GameEvent.h"
#include "../IGameEntity.h"
#include "../../input/InputConverter.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../../utils/Log.h"
#include "../../input/bindings/KeyToEventBind.h"
#include "../../input/bindings/MouseToEventBind.h"
#include "../GameEvent.h"
#include "../Memory.h"
#include "../InstantiableObject.h"

InputComponent::InputComponent()
{
}

InputComponent::~InputComponent()
{
}

InputComponent* InputComponent::DoClone() const
{
	return DBG_NEW InputComponent(*this);
}

std::shared_ptr<const GameEvent> InputComponent::ConvertKey(int key, int action) const
{
	for (const InputConverter* converter : mConverters)
	{
		std::shared_ptr<const GameEvent> event = converter->Convert(key, action);
		if (event != nullptr)
		{
			return event;
		}
	}

	return nullptr;
}

std::shared_ptr<const GameEvent> InputComponent::ConvertMouse(void* data) const
{
	for (const InputConverter* converter : mConverters)
	{
		std::shared_ptr<const GameEvent> event = converter->Convert(data);
		if (event != nullptr)
		{
			return event;
		}
	}

	return nullptr;
}

IComponent* InputComponent::Create(IGameEntity* entity)
{
	InputComponent* component = DBG_NEW InputComponent();
	entity->AddComponent(component);

	return component;
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

void InputComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	if (source->HasAttribute("converters"))
	{
		source->BeginAttribute(std::string("converters"));
		unsigned int numConverters = source->ReadNumberOfElements();
		source->BeginAttribute(std::string("converter"));
		do
		{
			InputConverter* converter = ReadConverterFrom(source);
			if (converter != nullptr)
			{
				AddConverter(converter);
			}
			source->NextAttribute();
			numConverters--;

		} while (numConverters > 0);
		source->EndAttribute();
		source->EndAttribute();
	}
}

InputConverter* InputComponent::ReadConverterFrom(core::utils::IDeserializer* source)
{
	int value = 0;
	std::string eventName;
	source->ReadParameter("event", eventName);

	std::shared_ptr<GameEvent> event = InstantiableObject::CreateGameEvent(eventName);
	if (event != nullptr)
	{
		if (source->ReadParameter("key", &value))
		{
			return DBG_NEW KeyToEventBind(value, event);
		}
		else if (source->ReadParameter("mouse", &value))
		{
			return DBG_NEW MouseToEventBind(value, event);
		}
	}
	else
	{
		Log(Log::LOG_ERROR) << "GameEvent " << eventName << " has not been created.\n";
	}
	return nullptr;
}

void InputComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("input_component"));
	destination->BeginAttribute(std::string("converters"));
	//TODO
	destination->EndAttribute();
}