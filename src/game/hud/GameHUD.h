#pragma once

class GameEntity;
class GameScene;
class ItemsListHUD;
class HealthHUD;

class GameHUD
{
	ItemsListHUD* mItemsList;
	HealthHUD* mHealthHUD;

public:
	GameHUD(GameScene* scene);
	~GameHUD();

	void SetVisibility(bool visibility);
	void Update(float elapsedTime);
	ItemsListHUD* GetItemsListHUD();
	HealthHUD* GetHealthHUD();

private:
	void Create(GameScene* scene);
};

