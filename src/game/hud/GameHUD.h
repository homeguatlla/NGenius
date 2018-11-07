#pragma once
#include "../../NGenius.h"

class GameEntity;
class GameScene;
class ItemsListHUD;

class GameHUD
{
	GameEntity* mGameHudGameEntity;
	ItemsListHUD* mItemsList;

public:
	GameHUD(NGenius& engine, GameScene* scene);
	~GameHUD();

	void Show();
private:
	void Create(NGenius& engine, GameScene* scene);
};

