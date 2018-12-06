#pragma once

#include <glm/glm.hpp>

class GameScene;
class GameEntity;
class Text;

class HealthHUD
{
	GameEntity* mBackgroundEntity;
	Text* mHealthTextEntity;
	glm::vec2 mScreenCoord;

public:
	HealthHUD(GameScene* scene, const glm::vec2& screenCoord);
	~HealthHUD();

private:
	void Create(GameScene* scene);
	void CreateBackground(GameScene* scene);
	void HealthHUD::CreateText(GameScene* scene);
};

