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

public:
	MarsPlanet(GameScene* scene, ICamera* gameplayCamera);
	~MarsPlanet();

private:
	void Create();
	void CreateTerrain();
	void CreateSky();
	void CreateRocks();

	GameEntity* CreateGameEntityFromModel(const std::string& modelName, Transformation* transformation, float introductionCoef);
};

