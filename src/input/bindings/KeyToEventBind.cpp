#include "stdafx.h"
#include "KeyToEventBind.h"
#include "../../resources/GameEvent.h"


KeyToEventBind::KeyToEventBind(int key, int action, const GameEvent* event) : 
	mKey(key), 
	mAction(action), 
	mEvent(event)
{
}


KeyToEventBind::~KeyToEventBind()
{
	delete mEvent;
}

const GameEvent* KeyToEventBind::Convert(int key, int action) const
{
	if (mKey == key && mAction == action)
	{
		return mEvent->Clone(nullptr);
	}
	else
	{
		return nullptr;
	}
}
