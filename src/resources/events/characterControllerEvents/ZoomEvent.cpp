#include "stdafx.h"
#include "ZoomEvent.h"
#include "../../systems/InputSystem.h"

ZoomEvent::ZoomEvent() : mZoom(0.0f)
{
}


ZoomEvent::~ZoomEvent()
{
}

std::shared_ptr<GameEvent> ZoomEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	std::shared_ptr<ZoomEvent> zoomEvent = std::make_shared<ZoomEvent>();
	zoomEvent->SetZoom(mouseData->mScroll);

	return zoomEvent;
}

std::shared_ptr<GameEvent> ZoomEvent::Create()
{
	return std::make_shared<ZoomEvent>();
}

void ZoomEvent::SetZoom(float zoom)
{
	mZoom = zoom;
}

float ZoomEvent::GetZoom() const
{
	return mZoom;
}