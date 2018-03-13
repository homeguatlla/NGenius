#include "stdafx.h"
#include "ZoomEvent.h"


ZoomEvent::ZoomEvent()
{
}


ZoomEvent::~ZoomEvent()
{
}

InputEvent* ZoomEvent::DoClone() const
{
	return new ZoomEvent();
}