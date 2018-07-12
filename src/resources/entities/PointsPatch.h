#pragma once
#include "../GameEntity.h"

class IMaterial;
class Model;
class Terrain;

class PointsPatch :	public GameEntity
{
	const Terrain* mTerrain;
	float mHeightMin;
	float mHeightMax;
	float mWide;
	float mLength;
	float mDensity;
	Model* mModel;

public:
	explicit PointsPatch(Transformation* transformation, IMaterial* material, const Terrain* terrain, float heightMin, float heightMax, float wide, float length, float density);
	~PointsPatch();

private:
	void Create();
};

