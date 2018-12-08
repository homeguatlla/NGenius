#include "stdafx.h"
#include "DieEvent.h"

DieEvent::DieEvent()
{
}


DieEvent::~DieEvent()
{
}

DieEvent* DieEvent::DoClone(const void* data) const
{
	DieEvent* dieEvent = new DieEvent();

	return dieEvent;
}