#pragma once
#include <glm/glm.hpp>
#include "../../../visitor/BaseVisitable.h"
#include "../../scene/IGameSceneListener.h"
#include <vector>
#include <memory>
#include <string>

class GameEntity;
class Terrain;
class SunLight;

class EnvironmentSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<GameEntity*> mEntities;
	std::vector<GameEntity*> mModificators;
	const Terrain* mTerrain;
	float mTimer;
	std::vector<glm::vec3> mModificatorsPositions;
	SunLight* mSunLight;

public:
	EnvironmentSystem();
	~EnvironmentSystem();

	void Start();
	void Update(float deltaTime);
	
	unsigned int GetNumberGameEntities() const;

	void SetTerrain(const Terrain* terrain);
	
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

	float GetTimer() const;

	void SetDayHour(float hour);

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

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	void RemoveEntityVector(GameEntity* entity, std::vector<GameEntity*>& vector);
	bool HasEnvironmentComponents(const GameEntity* entity) const;

	void UpdateModificatorsVector();

	void ApplyWind(GameEntity* entity);

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

