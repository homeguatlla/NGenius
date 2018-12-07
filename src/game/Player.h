#pragma once

#include "../resources/Transformation.h"

class NGenius;
class GameScene;
class Character;
class GameEntity;
class Transformation;
class InventoryController;
class HealthController;

class Player
{
	NGenius* mEngine;
	GameScene* mScene;
	Character* mCharacter;
	InventoryController *mInventoryController;
	HealthController* mHealthController;

public:
	Player(GameScene* scene, Transformation* transformation, InventoryController* inventoryController, HealthController* healthController);
	~Player();

	void Update(float elapsedTime);

	GameEntity* GetEntity();

private:
	void Create(Transformation* transformation);
};

