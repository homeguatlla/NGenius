#pragma once
#include "../../NGenius.h"
#include "../../resources/entities/Terrain.h"

class GameEntity;
class GameScene;
class ICamera;

class MarsPlanet
{
	NGenius& mEngine;
	GameScene* mScene;
	Terrain* mTerrain;
	ICamera* mGameplayCamera;
	GameEntity* mSkyBox;

public:
	MarsPlanet(NGenius& engine, GameScene* scene, ICamera* gameplayCamera);
	~MarsPlanet();

private:
	void Create();
	void CreateTerrain();
	void CreateSky();
};

