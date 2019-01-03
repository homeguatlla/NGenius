#pragma once
#include "../../resources/entities/Terrain.h"

#include <vector>

class NGenius;
class GameEntity;
class GameScene;
class ICamera;

class MarsPlanet
{
	NGenius* mEngine;
	GameScene* mScene;
	Terrain* mTerrain;
	ICamera* mGameplayCamera;
	GameEntity* mSkyBox;
	std::vector<GameEntity*> mPropsEntityList;
	std::vector<GameEntity*> mFloorEntityList;

public:
	MarsPlanet(GameScene* scene, ICamera* gameplayCamera);
	~MarsPlanet();

private:
	void Create();
	void CreateTerrain();
	void CreateSky();
	void CreateRocks();
	void CreateCamp();
	void CreateCampTest();
};

