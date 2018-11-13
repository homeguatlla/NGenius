#include "stdafx.h"
#include "ItemFactory.h"
#include "../../utils/Log.h"

ItemFactory::ItemFactory()
{
}


ItemFactory::~ItemFactory()
{
}

//TODO ojo!! los nombres que le damos a los items ahora mismo tienen que coincidir con el nombre de la textura
Item* ItemFactory::Create(Item::ItemType type)
{
	switch (type)
	{
	case Item::ITEM_SHOT_GUN:
		return new Item("item_shotgun", Item::ITEM_SHOT_GUN, 1, false, 1);
	case Item::ITEM_LIVE_CAPSULE:
		return new Item("item_water", Item::ITEM_LIVE_CAPSULE, 3, true, 12);
	case Item::ITEM_INVALID:
	default:
		Log(Log::LOG_ERROR) << "Error, unknown item type! \n";
		return nullptr;
	}
}