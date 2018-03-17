#include "stdafx.h"
#include "ForwardEvent.h"


ForwardEvent::ForwardEvent()
{
}


ForwardEvent::~ForwardEvent()
{
}

ForwardEvent* ForwardEvent::DoClone(const void* data) const
{
	return new ForwardEvent();
}
