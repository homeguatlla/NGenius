#pragma once
#include "../inventory/InventoryItem.h"

#include <glm/glm.hpp>

class GameEntity;
class GameScene;

class EntitiesFactory
{
	
public:
	EntitiesFactory();
	~EntitiesFactory();

	GameEntity* Create(InventoryItem::ItemType type, const glm::vec3& position, GameScene* scene);
};

