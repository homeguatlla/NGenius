#pragma once
#include "../BaseGameEntity.h"

class IMaterial;
class Model;
class Terrain;
class MaterialEffectFloat;

class PointsPatch :	public BaseGameEntity<PointsPatch>
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

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	
	void Build(NGenius* engine) override;
	static std::string GetClassName() { return std::string("terrain_points_patch"); }
	static IGameEntity* DoCreate();

private:
	PointsPatch() = default;
	void CreateModel();
	void CreatePointsPatch(IMaterial* material);

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

