#pragma once
#include "../items/Item.h"

#include <glm/glm.hpp>

class GameEntity;
class GameScene;

class EntitiesFactory
{
	
public:
	EntitiesFactory();
	~EntitiesFactory();

	GameEntity* Create(Item::ItemType type, const glm::vec3& position, GameScene* scene);
};

