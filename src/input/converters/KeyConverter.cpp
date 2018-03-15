#include "stdafx.h"
#include "KeyConverter.h"
#include "../../resources/GameEvent.h"


KeyConverter::KeyConverter(int key, int action, const GameEvent* event) : 
	mKey(key), 
	mAction(action), 
	mEvent(event)
{
}


KeyConverter::~KeyConverter()
{
	delete mEvent;
}

const GameEvent* KeyConverter::Convert(int key, int action) const
{
	if (mKey == key && mAction == action)
	{
		return mEvent->Clone();
	}
	else
	{
		return nullptr;
	}
}
