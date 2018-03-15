#include "stdafx.h"
#include "ZoomEvent.h"


ZoomEvent::ZoomEvent()
{
}


ZoomEvent::~ZoomEvent()
{
}

GameEvent* ZoomEvent::DoClone() const
{
	return new ZoomEvent();
}