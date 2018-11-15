#pragma once
#include "../../resources/GameEntity.h"
#include "../../NGenius.h"
#include "../items/Item.h"

class GameScene;
class Transformation;
class ParticlesEmitter;

class Battery :	public GameEntity
{
private:
	unsigned int mPower;
	Item::ItemType mType;
	ParticlesEmitter* mSteamParticleEmitter;

public:
	explicit Battery(Item::ItemType type, unsigned int power, GameEntity& entity);
	virtual ~Battery();

	Item::ItemType GetType() const;
	unsigned int GetPower() const;

	void AddIntoScene(GameScene* scene);
};

