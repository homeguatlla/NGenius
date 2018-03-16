#include "stdafx.h"
#include "ForwardEvent.h"


ForwardEvent::ForwardEvent()
{
}


ForwardEvent::~ForwardEvent()
{
}

ForwardEvent* ForwardEvent::DoClone() const
{
	return new ForwardEvent();
}
