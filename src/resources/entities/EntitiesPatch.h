#pragma once
#include "../BaseGameEntity.h"

class IMaterial;
class Model;
class Terrain;
class MaterialEffectFloat;
class NGenius;

class EntitiesPatch : public BaseGameEntity<EntitiesPatch>
{
	const Terrain* mTerrain;
	float mHeightMin;
	float mHeightMax;
	float mWide;
	float mLength;
	float mDensity;
	bool mIsIntersectionAllowed;
	std::vector<std::pair<std::string, int>> mModelsName; //name, percentage
	std::vector<std::pair<IGameEntity*, float>> mEntities; //entity, radius

public:
	explicit EntitiesPatch(Transformation* transformation, const Terrain* terrain, float heightMin, float heightMax, float wide, float length, float density, bool isIntersectionAllowed);
	~EntitiesPatch() = default;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	void Build(NGenius* engine) override;
	static std::string GetClassName() { return std::string("terrain_entities_patch"); }
	static IGameEntity* DoCreate();

private:
	EntitiesPatch() = default;
	void CreateEntities(NGenius* engine);
	void ReadModels(core::utils::IDeserializer* source);
	void FillEntitiesRadiusVector(NGenius* engine);
	int CalculateNumBiggerEntitiesFitArea();
	bool FillWithRandomPoint(glm::vec3& point);
	void SpawnEntities(NGenius* engine, int numEntities);
	bool CanPlaceEntityOnPoint(IGameEntity* entity, glm::vec3& point, std::vector<std::pair<glm::vec3, float>>& pointsWithEntityOver);

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

