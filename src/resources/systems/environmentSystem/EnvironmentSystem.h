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
	float mInitialTimer;
	std::vector<glm::vec3> mModificatorsPositions;
	SunLight* mSunLight;
	long mDayTime;

public:
	EnvironmentSystem();
	~EnvironmentSystem();

	void Start();
	void Update(float deltaTime);
	
	unsigned int GetNumberGameEntities() const;

	void SetTerrain(const Terrain* terrain);
	
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

	float GetTimer() const;
	void SetInitialTimer(float time);

	void SetDayTime(float hour);
	float GetDayTime() const;	

	float GetSunLightBlendFactor() const;
	glm::vec3 GetSunLightDirection() const;
	glm::vec3 GetSunLightColor() const;

	const std::string GetSkyBoxCubemapOrigin() const;
	const std::string GetSkyBoxCubemapDestination() const;

	void AddSunLightFrame(float hour, const float rotationAngle, const glm::vec3& color,
		const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemapName);

	//fog
	glm::vec3 GetFogColor() const;
	float GetFogDensity() const;
	float GetFogGradient() const;

	//debug
	void SetSunLightColor(const glm::vec3& color);
	void SetFogGradient(float gradient);

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	void RemoveEntityVector(GameEntity* entity, std::vector<GameEntity*>& vector);
	bool HasEnvironmentComponents(const GameEntity* entity) const;

	void UpdateModificatorsVector();

	void UpdateTime();
	void ApplyWind(GameEntity* entity);

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

