#pragma once

#include "../resources/Transformation.h"

class NGenius;
class GameScene;
class Character;
class GameEntity;
class Transformation;
class InventoryController;

class Player
{
	NGenius* mEngine;
	GameScene* mScene;
	Character* mCharacter;
	InventoryController *mInventoryController;

public:
	Player(GameScene* scene, Transformation* transformation, InventoryController* inventoryController);
	~Player();

	void Update(float elapsedTime);

	GameEntity* GetEntity();

private:
	void Create(Transformation* transformation);
};

