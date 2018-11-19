#include "stdafx.h"
#include "InventoryController.h"
#include "../hud/ItemsListHUD.h"
#include "../items/ItemFactory.h"
#include "../ShooterGameConstants.h"
#include "../Player.h"
#include "../hud/ItemHUD.h"
#include "../entities/EntitiesFactory.h"
#include "../components/PickedUpComponent.h"

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


InventoryController::InventoryController(GameScene* scene, Inventory* inventory, ItemsListHUD* itemsListHUD, Player* player) :
	mScene(scene),
	mInventory(inventory),
	mItemsHudList(itemsListHUD),
	mPlayer(player)/*,
	mLocalizedEntity(nullptr),
	mLocalizedOriginalMaterial(nullptr),
	mLocalizedMaterial(nullptr)*/
{
	NGenius::GetInstance().RegisterAllEventsInputListener(this);
}

InventoryController::~InventoryController()
{
	NGenius::GetInstance().UnRegisterInputListener(this);
}

/*void InventoryController::CreateLocalizedMaterial(const std::string& materialName, bool hasNormalmap)
{
	std::string localizedMaterialName = materialName + "_localized";
	mLocalizedMaterial = mEngine.GetMaterial(localizedMaterialName);
	if (mLocalizedMaterial == nullptr)
	{
		std::string shaderName = hasNormalmap ? "normalmap" : "model";
		mLocalizedMaterial = mEngine.CreateMaterial(localizedMaterialName, mEngine.GetShader(shaderName));
		mLocalizedMaterial->AddEffect(new MaterialEffectDiffuseTexture(
			static_cast<Texture*>(mEngine.GetTexture(materialName + "_diffuse")),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1
		));
		if(hasNormalmap)
		{
			mLocalizedMaterial->AddEffect(new MaterialEffectNormalTexture(
					static_cast<Texture*>(mEngine.GetTexture(materialName + "_normalmap")),
					1
				));
		}
		mLocalizedMaterial->AddEffect(new MaterialEffectShadowProperties(1));
		mLocalizedMaterial->AddEffect(new MaterialEffectClippingPlane());

		Log(Log::LOG_INFO) << "created material " << materialName << "\n";
	}
}
*/
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
/*
void InventoryController::ShowElementsCanBePickedup(const glm::vec3& position)
{
	std::vector<std::pair<GameEntity*, float>> entities;
	mEngine.FillWithGameEntitiesVisibleInsideRadius(position, PLAYER_PICKUP_RADIUS, entities, true);

	if (!entities.empty())
	{
		if (mLocalizedEntity != nullptr)
		{
			bool found = std::find_if(
				entities.begin(), 
				entities.end(), 
				[&](std::pair<GameEntity*, float> a) { return a.first == mLocalizedEntity; }) != entities.end();

			if (found)
			{
				return;
			}
		}

		for (std::pair<GameEntity*, float> pair : entities)
		{
			//if still the gameentity is visible
			if (pair.first->GetRenderer()->IsVisible())
			{
				PickedUpComponent* component = pair.first->GetComponent<PickedUpComponent>();
				if (component != nullptr)
				{
					//mostrar que se pueden recoger
					ShowElementHUB(pair.first);
					break;
				}
			}
		}
	}
	else
	{
		if (mLocalizedEntity != nullptr)
		{
			mLocalizedEntity->GetRenderer()->SetMaterial(mLocalizedOriginalMaterial);
			mLocalizedOriginalMaterial = nullptr;
			mLocalizedEntity = nullptr;
		}
	}
}

void InventoryController::ShowElementHUB(GameEntity* entity)
{
	if (mLocalizedEntity != nullptr)
	{
		mLocalizedEntity->GetRenderer()->SetMaterial(mLocalizedOriginalMaterial);
	}

	mLocalizedEntity = entity;
	mLocalizedOriginalMaterial = mLocalizedEntity->GetRenderer()->GetMaterial();

	Model* model = entity->GetRenderer()->GetModel();
	CreateLocalizedMaterial(model->GetMaterialName(), model->HasNormalmap());
	mLocalizedEntity->GetRenderer()->SetMaterial(mLocalizedMaterial);
}*/

Item* InventoryController::Retrieve()
{
	unsigned int itemId = mItemsHudList->GetSelectedItemId();
	mItemsHudList->RemoveSelectedItem();
	Item* item = mInventory->Retrieve(itemId);

	return item;
}

void InventoryController::Drop()
{
	Item* item = Retrieve();
	if (item != nullptr)
	{
		EntitiesFactory factory;
		glm::vec3 position = mPlayer->GetEntity()->GetTransformation()->GetPosition();
		factory.Create(item->GetType(), position, mScene);
	}
}

void InventoryController::PickUp(GameEntity* pickedupEntity)
{
	if (pickedupEntity->GetRenderer()->IsVisible())
	{
		PickedUpComponent* component = pickedupEntity->GetComponent<PickedUpComponent>();
		if (component != nullptr)
		{
			Item::ItemType type = component->GetType();

			if (type != Item::ITEM_INVALID)
			{
				ItemFactory factory;
				Item* itemFound = factory.Create(type);
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

void InventoryController::OnKey(int key, int action)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		std::vector<std::pair<GameEntity*, float>> entities;
		NGenius::GetInstance().FillWithGameEntitiesVisibleInsideRadius(mPlayer->GetEntity()->GetTransformation()->GetPosition(), PLAYER_PICKUP_RADIUS, entities, true);
		if (!entities.empty())
		{
			PickUp(entities[0].first);
		}
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		Drop();
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
