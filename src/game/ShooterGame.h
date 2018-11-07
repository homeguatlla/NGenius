#pragma once
#include "../src/NGenius.h"

class GameHUD;
class GameScene;

class ShooterGame 
{
	GameScene* mScene;
	GameHUD* mGameHUD;

public:
	ShooterGame();
	~ShooterGame();

	void Start(NGenius& engine);
	void Update(float elapsedTime);
};

