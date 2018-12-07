#pragma once

#include <glm/glm.hpp>

class GameScene;
class GameEntity;
class Text;
class HealthEvent;

class HealthHUD
{
	GameEntity* mBackgroundEntity;
	GameEntity* mHealthBarEntity;
	GameEntity* mHealthTransitionBarEntity;
	Text* mHealthTextEntity;
	glm::vec2 mScreenCoord;

public:
	HealthHUD(GameScene* scene, const glm::vec2& screenCoord);
	~HealthHUD();

	void OnHealthEvent(HealthEvent* health);

private:
	void Create(GameScene* scene);
	void CreateBackground(GameScene* scene);
	void CreateText(GameScene* scene);
	void CreateHealthBar(GameScene* scene);
	void UpdateHealthBar(float health);
};

