#pragma once
#include <glm/glm.hpp>
#include "src/visitor/BaseVisitable.h"
#include "src/resources/scene/IGameSceneListener.h"
#include <vector>
#include <memory>
#include <string>

class IGameEntity;
class Terrain;
class SunLight;

class EnvironmentSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<std::shared_ptr<IGameEntity>> mEntities;
	std::vector<std::shared_ptr<IGameEntity>> mModificators;
	std::shared_ptr<Terrain> mTerrain;
	float mTimer;
	std::vector<glm::vec3> mModificatorsPositions;
	SunLight* mSunLight;
	long mDayTime;
	glm::vec3 mSunLightDirection;

public:
	EnvironmentSystem();
	~EnvironmentSystem();

	void Start();
	void Update(float deltaTime);
	void Reload();

	unsigned int GetNumberGameEntities() const;

	void SetTerrain(const std::shared_ptr<Terrain> terrain);
	
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

	float GetTimer() const;

	void SetDayTime(float hour);
	float GetDayTime() const;
	

	float GetSunLightBlendFactor() const;
	glm::vec3 GetSunLightDirection() const;
	glm::vec3 GetSunLightColor() const;

	const std::string GetSkyBoxCubemapOrigin() const;
	const std::string GetSkyBoxCubemapDestination() const;

	//fog
	glm::vec3 GetFogColor() const;
	float GetFogDensity() const;
	float GetFogGradient() const;

	//debug
	void SetSunLightColor(const glm::vec3& color);
	void SetFogGradient(float gradient);

private:
	void Release();
	void AddEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntityVector(std::shared_ptr<IGameEntity> entity, std::vector<std::shared_ptr<IGameEntity>>& vector);
	bool HasEnvironmentComponents(const std::shared_ptr<IGameEntity> entity) const;

	void UpdateModificatorsVector();

	void UpdateTime();
	void ApplyWind(std::shared_ptr<IGameEntity> entity);

	void OnGameEntityAdded(std::shared_ptr<IGameEntity> entity) override;
	void OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity) override;
};

