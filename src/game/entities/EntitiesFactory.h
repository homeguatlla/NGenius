#pragma once
#include "../items/Item.h"

#include <glm/glm.hpp>

class GameEntity;
class NGenius;
class GameScene;

class EntitiesFactory
{
	NGenius* mEngine;

public:
	EntitiesFactory(NGenius* engine);
	~EntitiesFactory();

	GameEntity* Create(Item::ItemType type, const glm::vec3& position, GameScene* scene);
};

