#include "stdafx.h"
#include "ItemsListHUD.h"

#include <GLFW/glfw3.h>

#include "../../resources/GameEntity.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/scene/GameScene.h"

#include "../../resources/components/CharacterComponent.h"
#include "../../resources/components/InputComponent.h"

#include "../../resources/events/characterControllerEvents/ZoomEvent.h"
#include "../events/StoreIntoInventoryEvent.h"
#include "../../input/bindings/MouseToEventBind.h"
#include "../../input/bindings/KeyToEventBind.h"

#include "../ShooterGameConstants.h"

#include "ItemHUD.h"

#include <string>
#include <iostream>

const std::string ITEM_QUAD_TEXTURE("item_quad_base");
const std::string ITEM_QUAD_SELECTED_TEXTURE("item_quad_selected");

const std::string MATERIAL_ITEM("item_quad_base_material");
const std::string MATERIAL_ITEM_SELECTED("item_quad_selected_material");
const int ITEM_SIZE = 64;
const int NUM_ITEMS = 5;
const float OFFSET_BETWEEN_ITEMS = 0.07f;

const float LAYER_SELECTED_ITEM_Z = 1.0f;


const std::string ITEMS_AVAILABLE_LIST[2] = {	
												std::string("item_water"), 
												std::string("item_shotgun") };


ItemsListHUD::ItemsListHUD(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord) :
	mEngine(&engine),
	mSelectedItemEntity(nullptr),
	mScreenCentre(engine.GetScreenWidth(), -engine.GetScreenHeight()),
	mScreenCoord(screenCoord),
	mSelectedItem(0)
{
	Create(scene);
}


ItemsListHUD::~ItemsListHUD()
{
}

void ItemsListHUD::SetVisibility(bool visible)
{
	mSelectedItemEntity->GetRenderer()->SetVisibility(visible);
	for (unsigned int i = 0; i < mItemsList.size(); ++i)
	{
		mItemsList[i]->SetVisibility(visible);
	}
}

void ItemsListHUD::AddItem(unsigned int itemId)
{
	ItemHUD* item = mItemsList[mSelectedItem];
	assert(item != nullptr);

	IMaterial* material = CreateMaterial(ITEMS_AVAILABLE_LIST[itemId] + std::string("_material"), ITEMS_AVAILABLE_LIST[itemId]);
	item->SetItemMaterial(material);
}

void ItemsListHUD::Create(GameScene* scene)
{
	CreateItems(scene);
	CreateSelectedItem(scene);
}

void ItemsListHUD::Update(float elapsedTime)
{
	UpdateGameEvents();
}

void ItemsListHUD::UpdateGameEvents()
{
	CharacterComponent* characterComponent = mSelectedItemEntity->GetComponent<CharacterComponent>();
	while (characterComponent->HasEvents())
	{
		const GameEvent* event = characterComponent->ConsumeEvent();
		if (event->IsOfType<ZoomEvent>())
		{
			const ZoomEvent* zoomEvent = static_cast<const ZoomEvent*>(event);
			int increment = zoomEvent->GetZoom() < 0 ? -1 : 1;
			int newSelectedItem = (static_cast<int>(mSelectedItem) + increment) % NUM_ITEMS;
			if (newSelectedItem < 0)
			{
				newSelectedItem = NUM_ITEMS + newSelectedItem;
			}
			mSelectedItem = newSelectedItem;
			UpdateSelectedItemPosition();
		}
		else if (event->IsOfType<StoreIntoInventoryEvent>())
		{
			const StoreIntoInventoryEvent* storeIntoInventoryEvent = static_cast<const StoreIntoInventoryEvent*>(event);
			AddItem(storeIntoInventoryEvent->GetItemId());
		}
	}
}

void ItemsListHUD::UpdateSelectedItemPosition()
{
	Transformation* transformation = mSelectedItemEntity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();
	position.x = CalculateItemPosition(mSelectedItem).x;
	transformation->SetPosition(position);
}

glm::vec2 ItemsListHUD::CalculateItemPosition(unsigned int item)
{
	return mScreenCentre * glm::vec2(mScreenCoord.x + OFFSET_BETWEEN_ITEMS * item, mScreenCoord.y);
}

void ItemsListHUD::CreateItems(GameScene* scene)
{
	for (int i = 0; i < NUM_ITEMS; ++i)
	{
		glm::vec2 screenCoords = CalculateItemPosition(i);
		CreateItem(scene, screenCoords, MATERIAL_ITEM, ITEM_QUAD_TEXTURE);
	}
}

void ItemsListHUD::CreateSelectedItem(GameScene* scene)
{
	glm::vec2 screenCoords = CalculateItemPosition(mSelectedItem);
	IMaterial* material = CreateMaterial(MATERIAL_ITEM_SELECTED, ITEM_QUAD_SELECTED_TEXTURE);
	IRenderer* guiRenderer = new IndicesRenderer(mEngine->GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	mSelectedItemEntity = new GameEntity(
		new Transformation(glm::vec3(screenCoords.x, screenCoords.y, LAYER_SELECTED_ITEM_Z),
		glm::vec3(0.0f),
		glm::vec3(ITEM_SIZE)),
		guiRenderer);

	//adding control of mouse roller
	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_E, new StoreIntoInventoryEvent()));
	mSelectedItemEntity->AddComponent(inputComponent);
	mSelectedItemEntity->AddComponent(new CharacterComponent());

	scene->AddEntity(mSelectedItemEntity);
}

void ItemsListHUD::CreateItem(GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName)
{
	IMaterial* material = CreateMaterial(materialName, textureName);
	IRenderer* guiRenderer = new IndicesRenderer(mEngine->GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	ItemHUD* item = new ItemHUD(guiRenderer, screenCoord, ITEM_SIZE);
	mItemsList.push_back(item);
	scene->AddEntity(item->GetGameEntity());
}

IMaterial* ItemsListHUD::CreateMaterial(const std::string& materialName, const std::string& textureName)
{
	IMaterial* material = mEngine->GetMaterial(materialName);
	if (material == nullptr)
	{
		material = mEngine->CreateMaterial(materialName, mEngine->GetShader("gui"));
		material->AddEffect(new MaterialEffectDiffuseTexture(mEngine->GetTexture(textureName), glm::vec3(1.0f), 1.0f));
		std::cout << "material item hud created: " << materialName << "\n";
	}

	return material;
}