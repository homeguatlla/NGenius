#include "stdafx.h"
#include "MouseToEventBind.h"
#include "../../resources/GameEvent.h"
#include "../../resources/systems/InputSystem.h"


MouseToEventBind::MouseToEventBind(int button, int action, std::shared_ptr<GameEvent> event) :
	mButton(button), 
	mAction(action),
	mEvent(event)
{
}

MouseToEventBind::MouseToEventBind(int button, std::shared_ptr<GameEvent> event) :
	mButton(button),
	mAction(-1),
	mEvent(event)
{

}

std::shared_ptr<GameEvent> MouseToEventBind::Convert(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	if (mButton == mouseData->mButton)
	{
		return mEvent->Clone(data);
	}
	else if (mButton == -1 && mouseData->mIsCursorMoved)
	{
		return mEvent->Clone(data);
	}
	else
	{
		return nullptr;
	}
}
