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

	/*GameEntity* mLocalizedEntity;
	IMaterial* mLocalizedOriginalMaterial;

	IMaterial* mLocalizedMaterial;
	*/
public:
	InventoryController(GameScene* scene, Inventory* inventory, ItemsListHUD* itemsListHUD);
	~InventoryController();

	//void ShowElementsCanBePickedup(const glm::vec3& position);
	void OnPickUp(const PickupGameEntityEvent* pickupEvent);
	void OnDrop(DropItemInventoryEvent* dropEvent);
	void OnNextPreviousItem(NextPreviousInventoryItemEvent* nextPreviousEvent);

private:
	//void CreateLocalizedMaterial(const std::string& materialName, bool hasNormalmap);
	//void ShowElementHUB(GameEntity* entity);
	
	void Store(InventoryItem* item);
	InventoryItem* Retrieve();
};

