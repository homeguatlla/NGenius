#include "stdafx.h"
#include "KeyConverter.h"
#include "../InputEvent.h"


KeyConverter::KeyConverter(int key, int action, const InputEvent* event) : 
	mKey(key), 
	mAction(action), 
	mEvent(event)
{
}


KeyConverter::~KeyConverter()
{
	delete mEvent;
}

const InputEvent* KeyConverter::Convert(int key, int action) const
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
