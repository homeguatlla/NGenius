#pragma once
#include "../../NGenius.h"

class GameEntity;
class GameScene;

class ItemsListHUD
{
	std::vector<GameEntity*> mItemsListEntity;
	GameEntity* mSelectedItemEntity;
	glm::vec2 mScreenCentre;
	glm::vec2 mScreenCoord;
	unsigned int mSelectedItem;

public:
	ItemsListHUD(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord);
	~ItemsListHUD();

	void Show();
	void Update(float elapsedTime);

private:
	void Create(NGenius& engine, GameScene* scene);
	void CreateItem(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName);
	GameEntity* CreateItemBase(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName);
	void CreateItems(NGenius& engine, GameScene* scene);
	void CreateSelectedItem(NGenius& engine, GameScene* scene);

	void UpdateGameEvents();
	void UpdateSelectedItemPosition();

	glm::vec2 CalculateItemPosition(unsigned int item);
};

