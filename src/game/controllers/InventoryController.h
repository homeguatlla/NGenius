#pragma once

#include "../../input/IInputListener.h"
#include "../inventory/Inventory.h"

class NGenius;
class ItemsListHUD;
class GameEntity;
class IMaterial;
class Player;
class GameScene;

class InventoryController : public IInputListener
{
	Inventory* mInventory;
	ItemsListHUD* mItemsHudList;
	NGenius* mEngine;
	GameScene* mScene;
	Player* mPlayer;

	/*GameEntity* mLocalizedEntity;
	IMaterial* mLocalizedOriginalMaterial;

	IMaterial* mLocalizedMaterial;
	*/
public:
	InventoryController(NGenius* engine, GameScene* scene, Inventory* inventory, ItemsListHUD* itemsListHUD, Player* player);
	~InventoryController();

	//void ShowElementsCanBePickedup(const glm::vec3& position);

private:
	//void CreateLocalizedMaterial(const std::string& materialName, bool hasNormalmap);
	//void ShowElementHUB(GameEntity* entity);
	
	void PickUp(GameEntity* pickedupEntity);
	void Drop();

	void Store(Item* item);
	Item* Retrieve();

	// Heredado vía IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseCursorPos(double x, double y) override;
};

