#pragma once
#include "../../resources/components/IComponent.h"
#include "../inventory/InventoryItem.h"

class PickupGameEntityComponent : public IComponent
{
	InventoryItem::ItemType mType;
	unsigned int mValue;

public:
	PickupGameEntityComponent(InventoryItem::ItemType type, unsigned int value);
	virtual ~PickupGameEntityComponent();

	InventoryItem::ItemType GetType() const;
	unsigned int GetValue() const;

	// Heredado vía IComponent
	IComponent * DoClone() const override;
};

