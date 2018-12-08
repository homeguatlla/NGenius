#pragma once

#include <glm/glm.hpp>

class GameScene;
class GameEntity;
class HealthEvent;
class HorizontalBar;

class HealthHUD
{
	GameEntity* mBackgroundEntity;
	glm::vec2 mScreenCoord;
	HorizontalBar* mHorizontalBar;

public:
	HealthHUD(GameScene* scene, const glm::vec2& screenCoord);
	~HealthHUD();

	void OnHealthEvent(const HealthEvent* health);

private:
	void Create(GameScene* scene);
	void CreateBackground(GameScene* scene);
	void CreateHorizontalBar(GameScene* scene);
};

