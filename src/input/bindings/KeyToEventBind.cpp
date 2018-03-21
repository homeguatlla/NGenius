#include "stdafx.h"
#include "KeyToEventBind.h"
#include "../../resources/GameEvent.h"


KeyToEventBind::KeyToEventBind(int key, const GameEvent* event) : 
	mKey(key), 
	mEvent(event)
{
}


KeyToEventBind::~KeyToEventBind()
{
	delete mEvent;
}

const GameEvent* KeyToEventBind::Convert(int key, int action) const
{
	if (mKey == key)
	{
		const int* keyData = reinterpret_cast<const int*>(action);
		return mEvent->Clone(keyData);
	}
	else
	{
		return nullptr;
	}
}
