#pragma once
#include "../BaseGameEntity.h"

class IMaterial;
class Model;
class Terrain;
class MaterialEffectFloat;

class PointsPatch :	public BaseGameEntity<PointsPatch>
{
	std::shared_ptr<Terrain> mTerrain;
	float mHeightMin;
	float mHeightMax;
	float mWide;
	float mLength;
	float mDensity;
	Model* mModel;

public:
	PointsPatch() = default;
	explicit PointsPatch(Transformation* transformation, IMaterial* material, const std::shared_ptr<Terrain> terrain, float heightMin, float heightMax, float wide, float length, float density);
	~PointsPatch() = default;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	
	void Build(NGenius* engine) override;
	static std::string GetClassName() { return std::string("terrain_points_patch"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	
	void CreateModel();
	void CreatePointsPatch(IMaterial* material);

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

