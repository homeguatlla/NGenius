#include "stdafx.h"
#include "ShooterGame.h"
#include "hud/GameHUD.h"
#include "ShooterGameConstants.h"

ShooterGame::ShooterGame()
{
}


ShooterGame::~ShooterGame()
{
}

void ShooterGame::Start(NGenius& engine)
{
	mScene = engine.CreateGameScene(GAME_SCENE_NAME);
	mGameHUD = new GameHUD(engine, mScene);
	mGameHUD->Show();
}

void ShooterGame::Update(float elapsedTime)
{
	mGameHUD->Update(elapsedTime);
}