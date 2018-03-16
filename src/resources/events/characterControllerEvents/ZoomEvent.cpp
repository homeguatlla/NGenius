#include "stdafx.h"
#include "ZoomEvent.h"


ZoomEvent::ZoomEvent() : mZoom(0.0f)
{
}


ZoomEvent::~ZoomEvent()
{
}

ZoomEvent* ZoomEvent::DoClone() const
{
	return new ZoomEvent();
}

void ZoomEvent::SetFloatValue(float value)
{
	mZoom = value;
}

float ZoomEvent::GetZoom() const
{
	return mZoom;
}