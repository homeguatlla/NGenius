#pragma once
#include "../../resources/components/IComponent.h"
#include "../items/Item.h"

class PickedUpComponent : public IComponent
{
	Item::ItemType mType;
	unsigned int mValue;

public:
	PickedUpComponent(Item::ItemType type, unsigned int value);
	virtual ~PickedUpComponent();

	Item::ItemType GetType() const;
	unsigned int GetValue() const;

	// Heredado vía IComponent
	IComponent * DoClone() const override;
};

