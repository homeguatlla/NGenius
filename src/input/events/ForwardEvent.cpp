#include "stdafx.h"
#include "ForwardEvent.h"


ForwardEvent::ForwardEvent()
{
}


ForwardEvent::~ForwardEvent()
{
}

InputEvent* ForwardEvent::DoClone() const
{
	return new ForwardEvent();
}
