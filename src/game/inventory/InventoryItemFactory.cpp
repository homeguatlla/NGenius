#include "stdafx.h"
#include "InventoryItemFactory.h"
#include "../../utils/Log.h"

ItemFactory::ItemFactory()
{
}


ItemFactory::~ItemFactory()
{
}

//TODO ojo!! los nombres que le damos a los items ahora mismo tienen que coincidir con el nombre de la textura
InventoryItem* ItemFactory::Create(InventoryItem::ItemType type)
{
	switch (type)
	{
	case InventoryItem::ITEM_SHOT_GUN:
		return new InventoryItem("item_shotgun", InventoryItem::ITEM_SHOT_GUN, 1, false, 1);
	case InventoryItem::ITEM_WATER_BATTERY:
		return new InventoryItem("item_water", InventoryItem::ITEM_WATER_BATTERY, 3, true, 12);
	case InventoryItem::ITEM_INVALID:
	default:
		Log(Log::LOG_ERROR) << "Error, unknown item type! \n";
		return nullptr;
	}
}