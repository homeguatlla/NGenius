#pragma once

#include "../inventory/InventoryItem.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

class GameScene;
class ItemHUD;
class GameEntity;
class IMaterial;
class NextPreviousInventoryItemEvent;

class ItemsListHUD
{
	std::vector<ItemHUD*> mItemsList;
	GameEntity* mSelectedItemEntity;
	glm::vec2 mScreenCoord;
	unsigned int mSelectedItem;
	unsigned int mNumItems;

public:
	ItemsListHUD(GameScene* scene, const glm::vec2& screenCoord, unsigned int numItems);
	~ItemsListHUD();

	void SetVisibility(bool visible);
	void AddItem(InventoryItem* item);
	void RemoveSelectedItem();
	unsigned int GetSelectedItemId() const;

	void OnNextPreviousItem(NextPreviousInventoryItemEvent* nextPreviousEvent);

private:
	void Create(GameScene* scene);
	void CreateItem(GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName);
	void CreateItems(GameScene* scene);
	void CreateSelectedItem(GameScene* scene);

	void UpdateSelectedItemPosition(unsigned int selectedItem, unsigned int newSelectedItem);

	glm::vec2 CalculateItemPosition(unsigned int item);

	void SetSize(GameEntity* entity, int size);
	bool IsItemHUDEmpty(ItemHUD* itemHUD) const;
};

