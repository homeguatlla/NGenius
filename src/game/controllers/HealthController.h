#pragma once

#include "../../input/IInputListener.h"

class HealthHUD;
class HealthEvent;

class GameScene;

class HealthController
{
	HealthHUD* mHealthHUD;
	GameScene* mScene;

public:
	HealthController(GameScene* scene, HealthHUD* healthHUD);
	~HealthController();

	void OnHealthEvent(const HealthEvent* healthEvent);
};

