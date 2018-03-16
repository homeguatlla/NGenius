#include "stdafx.h"
#include "StopEvent.h"


StopEvent::StopEvent()
{
}


StopEvent::~StopEvent()
{
}

StopEvent* StopEvent::DoClone() const
{
	return new StopEvent();
}
