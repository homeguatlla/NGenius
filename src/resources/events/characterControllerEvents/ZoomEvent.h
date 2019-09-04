#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class ZoomEvent : public CharacterControllerEvent
{
	float mZoom;

public:
	ZoomEvent();
	~ZoomEvent();

	std::shared_ptr<GameEvent> DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("ZoomEvent"); }
	static std::shared_ptr<GameEvent> Create();

	float GetZoom() const;
	void SetZoom(float zoom);
};

