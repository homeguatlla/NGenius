#pragma once
#include "../BaseGameEntity.h"

class IMaterial;
class Model;
class Terrain;
class MaterialEffectFloat;
class NGenius;

class EntitiesPatch : public BaseGameEntity<EntitiesPatch>
{
	using ModelPack = std::tuple<std::vector<std::string>, float, int>; //list of models , max radius all models, percentage
	typedef std::map<std::string, std::pair<std::shared_ptr<IGameEntity>, float>>::iterator EntitiesListIterator;

	std::shared_ptr<Terrain> mTerrain;
	float mHeightMin;
	float mHeightMax;
	float mWide;
	float mLength;
	float mDensity;
	bool mIsIntersectionAllowed;
	std::vector<ModelPack> mModelsName;
	std::map<std::string, std::pair<std::shared_ptr<IGameEntity>, float>> mEntities; //entity, radius

public:
	EntitiesPatch() = default;
	explicit EntitiesPatch(Transformation* transformation, const std::shared_ptr<Terrain> terrain, float heightMin, float heightMax, float wide, float length, float density, bool isIntersectionAllowed);
	~EntitiesPatch() = default;

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;

	void Build(NGenius* engine) override;
	static std::string GetClassName() { return std::string("terrain_entities_patch"); }
	static std::shared_ptr<IGameEntity> DoCreate();

private:
	
	void CreateEntities(GameScene* scene);
	void ReadModels(core::utils::IDeserializer* source);
	void FillEntitiesRadiusVector(GameScene* scene);
	int CalculateNumBiggerEntitiesFitArea();
	bool FillWithRandomPoint(glm::vec3& point);
	void SpawnEntities(GameScene* scene, int numEntities);
	bool CanPlaceEntityOnPoint(float radius, glm::vec3& point, std::vector<std::pair<glm::vec3, float>>& pointsWithEntityOver);

	// Heredado vía BaseGameEntity
	virtual void DoInit(GameScene* scene, RenderSystem* renderSystem) override;
};

