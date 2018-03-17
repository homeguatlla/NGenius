#include "stdafx.h"
#include "MouseConverter.h"
#include "../../resources/GameEvent.h"
#include "../../resources/systems/InputSystem.h"


MouseConverter::MouseConverter(int button, int action, const GameEvent* event) :
	mButton(button), 
	mAction(action),
	mEvent(event)
{
}

MouseConverter::MouseConverter(int button, const GameEvent* event) :
	mButton(button),
	mAction(-1),
	mEvent(event)
{

}

MouseConverter::~MouseConverter()
{
	delete mEvent;
}

const GameEvent* MouseConverter::Convert(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	if (mButton == mouseData->mButton)
	{
		return mEvent->Clone(data);
	}
	else
	{
		return nullptr;
	}
}
