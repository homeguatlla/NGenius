#pragma once

#include "../resources/Transformation.h"

class NGenius;
class GameScene;
class Character;
class GameEntity;
class Transformation;

class Player
{
	NGenius* mEngine;
	GameScene* mScene;
	Character* mCharacter;

public:
	Player(NGenius* engine, GameScene* scene, Transformation* transformation);
	~Player();

	GameEntity* GetEntity();

private:
	void Create(Transformation* transformation);
};

