#pragma once
#include "../../NGenius.h"

class GameEntity;
class GameScene;
class ItemsListHUD;

class GameHUD
{
	GameEntity* mGameHudGameEntity;
	ItemsListHUD* mItemsList;
	NGenius& mEngine;

public:
	GameHUD(NGenius& engine, GameScene* scene);
	~GameHUD();

	void SetVisibility(bool visibility);
	void Update(float elapsedTime);
	ItemsListHUD* GetItemsListHUD();

private:
	void Create(GameScene* scene);
};

