#include "stdafx.h"
#include "ForwardEvent.h"


ForwardEvent::ForwardEvent()
{
}


ForwardEvent::~ForwardEvent()
{
}

GameEvent* ForwardEvent::DoClone() const
{
	return new ForwardEvent();
}
