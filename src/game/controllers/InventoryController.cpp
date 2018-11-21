#include "stdafx.h"
#include "InventoryController.h"
#include "../hud/ItemsListHUD.h"
#include "../inventory/InventoryItemFactory.h"
#include "../ShooterGameConstants.h"
#include "../Player.h"
#include "../hud/ItemHUD.h"
#include "../entities/EntitiesFactory.h"
#include "../components/PickupGameEntityComponent.h"
#include "../events/PickupGameEntityEvent.h"
#include "../events/DropItemInventoryEvent.h"

#include "../../NGenius.h"
#include "../../resources/renderers/IRenderer.h"

#include "../../resources/GameEntity.h"
#include "../../resources/scene/GameScene.h"

#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectNormalTexture.h"
#include "../../resources/materials/effects/MaterialEffectShadowProperties.h"
#include "../../resources/materials/effects/MaterialEffectClippingPlane.h"

#include "../../resources/textures/ITexture.h"
#include "../../resources/textures/Texture.h"

#include "../../resources/models/Model.h"

#include "../../utils/Log.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <algorithm>


InventoryController::InventoryController(GameScene* scene, Inventory* inventory, ItemsListHUD* itemsListHUD) :
	mScene(scene),
	mInventory(inventory),
	mItemsHudList(itemsListHUD)
{
	
}

InventoryController::~InventoryController()
{
	
}

void InventoryController::Store(InventoryItem* item)
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

InventoryItem* InventoryController::Retrieve()
{
	unsigned int itemId = mItemsHudList->GetSelectedItemId();
	mItemsHudList->RemoveSelectedItem();
	InventoryItem* item = mInventory->Retrieve(itemId);

	return item;
}

void InventoryController::OnDrop(DropItemInventoryEvent* dropEvent)
{
	InventoryItem* item = Retrieve();
	if (item != nullptr)
	{
		EntitiesFactory factory;
		factory.Create(item->GetType(), dropEvent->GetPosition(), mScene);
	}
}

void InventoryController::OnPickUp(const PickupGameEntityEvent* pickupEvent)
{
	if (pickupEvent != nullptr)
	{
		GameEntity* pickedupEntity = pickupEvent->GetGameEntity();

		if (pickedupEntity->GetRenderer()->IsVisible())
		{
			PickupGameEntityComponent* component = pickedupEntity->GetComponent<PickupGameEntityComponent>();
			if (component != nullptr)
			{
				InventoryItem::ItemType type = component->GetType();

				if (type != InventoryItem::ITEM_INVALID)
				{
					ItemFactory factory;
					InventoryItem* itemFound = factory.Create(type);
					if (itemFound != nullptr)
					{
						Store(itemFound);
						//entonces destroy GameEntity
						//TODO El inventory controller debería trabajar con objetos de una classe que tuvieran una referencia a escena etc
						//y que simplemente al hacer un delete se sepan quitar de las escenas etc..
						//al igual que sepan añadirse 
						mScene->RemoveEntity(pickedupEntity);
					}
				}
			}
		}
	}
}

void InventoryController::OnNextPreviousItem(NextPreviousInventoryItemEvent* nextPreviousEvent)
{
	mItemsHudList->OnNextPreviousItem(nextPreviousEvent);
}

