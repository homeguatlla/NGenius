#include "stdafx.h"
#include "BackwardEvent.h"


BackwardEvent::BackwardEvent()
{
}


BackwardEvent::~BackwardEvent()
{
}

BackwardEvent* BackwardEvent::DoClone(const void* data) const
{
	return new BackwardEvent();
}