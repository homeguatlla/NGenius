#include "stdafx.h"
#include "InventoryController.h"
#include "../hud/ItemsListHUD.h"
#include "../items/ItemFactory.h"

#include <GLFW/glfw3.h>

InventoryController::InventoryController(NGenius& engine, Inventory* inventory, ItemsListHUD* itemsListHUD) :
	mInventory(inventory),
	mItemsHudList(itemsListHUD),
	mEngine(engine)
{
	mEngine.RegisterAllEventsInputListener(this);
}


InventoryController::~InventoryController()
{
	mEngine.UnRegisterInputListener(this);
}


void InventoryController::Store(Item* item)
{
	if (!item->IsAccumulative())
	{
		if (!mInventory->HasSpace())
		{
			//replace
			unsigned int itemId = mItemsHudList->GetSelectedItemId();
			mInventory->Retrieve(itemId);
			mItemsHudList->RemoveSelectedItem();
		}
		assert(mInventory->HasSpace());
		mInventory->Store(item);
		mItemsHudList->AddItem(item);
	}
	else
	{
		unsigned int remainder = 1;

		if (mInventory->ExistItem(item->GetType()))
		{
			//try to accumulate
		}

		if (remainder > 0)
		{
			//store the rest or stor if cannot accumulate
			if (!mInventory->HasSpace())
			{
				//TODO si hemos acumulado, y NO tenemos seleccionado el elemento acumulado reemplazamos
				//sino, estaríamos perdiendo items
				//recojo 7 tenía 5, máximo es 10. Después de acumular tengo 2 por recoger y 10 acumulados
				//si reemplazo, me quedo con 2 en lugar de 10
				unsigned int itemId = mItemsHudList->GetSelectedItemId();
				mInventory->Retrieve(itemId);
				mItemsHudList->RemoveSelectedItem();
			}
			
			assert(mInventory->HasSpace());
			mInventory->Store(item);
			mItemsHudList->AddItem(item);
		}
	}
}

void InventoryController::OnKey(int key, int action)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		//TODO buscar posibles items cerca
		ItemFactory factory;
		Item::ItemType type = mInventory->HasSpace() ? Item::ITEM_SHOT_GUN : Item::ITEM_LIVE_CAPSULE;
		Item* itemFound = factory.Create(type);
		if (itemFound != nullptr)
		{
			Store(itemFound);
		}
	}
}

void InventoryController::OnMouseScroll(int button, float scroll)
{
}

void InventoryController::OnMouseButton(int button, int action, int mods)
{
}

void InventoryController::OnMouseCursorPos(double x, double y)
{
}
