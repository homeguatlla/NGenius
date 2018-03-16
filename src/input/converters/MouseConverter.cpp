#include "stdafx.h"
#include "MouseConverter.h"
#include "../../resources/GameEvent.h"


MouseConverter::MouseConverter(int button, int action, const GameEvent* event) :
	mButton(button), 
	mAction(action),
	mScroll(0.0f),
	mEvent(event)
{
}

MouseConverter::MouseConverter(int button, const GameEvent* event) :
	mButton(button),
	mAction(-1),
	mScroll(0.0f),
	mEvent(event)
{

}

MouseConverter::~MouseConverter()
{
	delete mEvent;
}

const GameEvent* MouseConverter::Convert(int button, int action) const
{
	if (mButton == button && mAction == action)
	{
		return mEvent->Clone();
	}
	else
	{
		return nullptr;
	}
}

const GameEvent* MouseConverter::Convert(int button, float scroll) const
{
	if (mButton == button)
	{
		GameEvent* event = mEvent->Clone();
		event->SetFloatValue(scroll);
		return event;
	}
	else
	{
		return nullptr;
	}
}
