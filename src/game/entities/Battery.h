#pragma once
#include "../../resources/GameEntity.h"
#include "../../NGenius.h"
#include "../inventory/InventoryItem.h"

class Transformation;
class GameScene;

class Battery :	public GameEntity
{
private:
	unsigned int mPower;
	InventoryItem::ItemType mType;

public:
	explicit Battery(InventoryItem::ItemType type, unsigned int power, GameEntity& entity);
	virtual ~Battery();

	InventoryItem::ItemType GetType() const;
	unsigned int GetPower() const;
};

