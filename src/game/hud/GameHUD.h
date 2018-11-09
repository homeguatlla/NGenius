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

	void Show();
	void Update(float elapsedTime);

private:
	void Create(GameScene* scene);
};

