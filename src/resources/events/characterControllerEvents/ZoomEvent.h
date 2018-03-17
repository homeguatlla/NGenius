#pragma once
#include "../CharacterControllerEvent.h"

class ZoomEvent : public CharacterControllerEvent
{
	float mZoom;

public:
	ZoomEvent();
	~ZoomEvent();

	ZoomEvent* DoClone(const void* data) const override;
	
	float GetZoom() const;
	void SetZoom(float zoom);
};

