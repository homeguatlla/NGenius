#include "stdafx.h"
#include "ZoomEvent.h"
#include "../../systems/InputSystem.h"

ZoomEvent::ZoomEvent() : mZoom(0.0f)
{
}


ZoomEvent::~ZoomEvent()
{
}

ZoomEvent* ZoomEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	ZoomEvent* zoomEvent = new ZoomEvent();
	zoomEvent->SetZoom(mouseData->mScroll);

	return zoomEvent;
}

void ZoomEvent::SetZoom(float zoom)
{
	mZoom = zoom;
}

float ZoomEvent::GetZoom() const
{
	return mZoom;
}