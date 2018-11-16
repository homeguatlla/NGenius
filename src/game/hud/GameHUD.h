#pragma once

class NGenius;
class GameEntity;
class GameScene;
class ItemsListHUD;


class GameHUD
{
	ItemsListHUD* mItemsList;
	NGenius* mEngine;
	

public:
	GameHUD(NGenius* engine, GameScene* scene);
	~GameHUD();

	void SetVisibility(bool visibility);
	void Update(float elapsedTime);
	ItemsListHUD* GetItemsListHUD();

private:
	void Create(GameScene* scene);
};

