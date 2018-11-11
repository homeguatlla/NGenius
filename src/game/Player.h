#pragma once
#include "../NGenius.h"
#include "../resources/Transformation.h"

class GameScene;
class Character;

class Player
{
	NGenius& mEngine;
	GameScene* mScene;
	Character* mCharacter;

public:
	Player(NGenius& engine, GameScene* scene, Transformation* transformation);
	~Player();

private:
	void Create(Transformation* transformation);
};

