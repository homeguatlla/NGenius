#pragma once
#include "../../resources/GameEntity.h"
#include "../../NGenius.h"
#include "../inventory/InventoryItem.h"

class Transformation;
class ParticlesEmitter;

class Battery :	public GameEntity
{
private:
	unsigned int mPower;
	InventoryItem::ItemType mType;
	ParticlesEmitter* mSteamParticleEmitter;

public:
	explicit Battery(InventoryItem::ItemType type, unsigned int power, GameEntity& entity, ParticlesEmitter* emitter);
	virtual ~Battery();

	InventoryItem::ItemType GetType() const;
	unsigned int GetPower() const;

	ParticlesEmitter* GetParticlesEmitter();
};

