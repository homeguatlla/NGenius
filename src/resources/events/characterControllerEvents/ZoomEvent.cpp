#include "stdafx.h"
#include "ZoomEvent.h"


ZoomEvent::ZoomEvent()
{
}


ZoomEvent::~ZoomEvent()
{
}

ZoomEvent* ZoomEvent::DoClone() const
{
	return new ZoomEvent();
}