#pragma once

class GameEntity;
class GameScene;
class ItemsListHUD;


class GameHUD
{
	ItemsListHUD* mItemsList;

public:
	GameHUD(GameScene* scene);
	~GameHUD();

	void SetVisibility(bool visibility);
	void Update(float elapsedTime);
	ItemsListHUD* GetItemsListHUD();

private:
	void Create(GameScene* scene);
};

