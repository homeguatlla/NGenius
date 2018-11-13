#pragma once

#include "../../input/IInputListener.h"
#include "../inventory/Inventory.h"
#include "../../NGenius.h"

class ItemsListHUD;

class InventoryController : public IInputListener
{
	Inventory* mInventory;
	ItemsListHUD* mItemsHudList;
	NGenius mEngine;

public:
	InventoryController(NGenius& engine, Inventory* inventory, ItemsListHUD* itemsListHUD);
	~InventoryController();

	void Store(Item* item);

private:
	// Heredado vía IInputListener
	void OnKey(int key, int action) override;
	void OnMouseScroll(int button, float scroll) override;
	void OnMouseButton(int button, int action, int mods) override;
	void OnMouseCursorPos(double x, double y) override;
};

