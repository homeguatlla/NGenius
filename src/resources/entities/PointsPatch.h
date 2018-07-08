#pragma once
#include "../GameEntity.h"

class IMaterial;
class Model;
class Terrain;

class PointsPatch :	public GameEntity
{
	const Terrain* mTerrain;
	float mWaterHeight;
	float mWide;
	float mLength;
	float mDensity;
	Model* mModel;

public:
	explicit PointsPatch(Transformation* transformation, IMaterial* material, const Terrain* terrain, float waterHeight, float wide, float length, float density);
	~PointsPatch();

private:
	void Create();
};

