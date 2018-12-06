#pragma once

#include "../../input/IInputListener.h"
#include "../inventory/Inventory.h"

class ItemsListHUD;
class PickupGameEntityEvent;
class DropItemInventoryEvent;
class NextPreviousInventoryItemEvent;
class IMaterial;
class GameScene;

class InventoryController
{
	Inventory* mInventory;
	ItemsListHUD* mItemsHudList;
	GameScene* mScene;

public:
	InventoryController(GameScene* scene, Inventory* inventory, ItemsListHUD* itemsListHUD);
	~InventoryController();

	void OnPickUp(const PickupGameEntityEvent* pickupEvent);
	void OnDrop(DropItemInventoryEvent* dropEvent);
	void OnNextPreviousItem(NextPreviousInventoryItemEvent* nextPreviousEvent);

private:
	void Store(InventoryItem* item);
	InventoryItem* Retrieve();
};

