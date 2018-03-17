#include "stdafx.h"
#include "StopEvent.h"


StopEvent::StopEvent()
{
}


StopEvent::~StopEvent()
{
}

StopEvent* StopEvent::DoClone(const void* data) const
{
	return new StopEvent();
}
