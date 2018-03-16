#pragma once
#include "../CharacterControllerEvent.h"

class ZoomEvent : public CharacterControllerEvent
{
	float mZoom;

public:
	ZoomEvent();
	~ZoomEvent();

	ZoomEvent* DoClone() const override;
	void SetFloatValue(float value) override;

	float GetZoom() const;
};

