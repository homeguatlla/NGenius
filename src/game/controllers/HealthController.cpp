#include "stdafx.h"
#include "HealthController.h"
#include "../hud/HealthHUD.h"
#include "../Player.h"
#include "../../resources/events/characterControllerEvents/HealthEvent.h"

#include "../../NGenius.h"

#include "../../resources/GameEntity.h"
#include "../../resources/scene/GameScene.h"

#include <GLFW/glfw3.h>
#include <vector>


HealthController::HealthController(GameScene* scene, HealthHUD* healthHUD) :
	mScene(scene),
	mHealthHUD(healthHUD)
{
	
}

HealthController::~HealthController()
{
	
}

void HealthController::OnHealthEvent(const HealthEvent* event)
{
	mHealthHUD->OnHealthEvent(event);
}

