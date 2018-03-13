#include "stdafx.h"
#include "KeyConverter.h"


KeyConverter::KeyConverter(int key, int action, const InputEvent* event) : 
	mKey(key), 
	mAction(action), 
	mEvent(event)
{
}


KeyConverter::~KeyConverter()
{
}

const InputEvent* KeyConverter::Convert(int key, int action) const
{
	if (mKey == key && mAction == action)
	{
		return mEvent;
	}
	else
	{
		return nullptr;
	}
}
