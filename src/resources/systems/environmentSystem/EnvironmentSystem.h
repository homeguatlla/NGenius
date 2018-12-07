#pragma once
#include <glm/glm.hpp>
#include "../ISystem.h"
#include <vector>
#include <memory>
#include <string>

class GameEntity;
class Terrain;
class SunLight;

class EnvironmentSystem : public ISystem
{
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

	void Start() override;
	void Update(float deltaTime) override;
	bool HasToBeRegisteredToGameScene() const override;
	
	unsigned int GetNumberGameEntities() const;

	void SetTerrain(const Terrain* terrain);
	
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
	void AddEntity(GameEntity* entity) override;
	void RemoveEntity(GameEntity* entity) override;

	void RemoveEntityVector(GameEntity* entity, std::vector<GameEntity*>& vector);
	bool HasSpecificComponents(const GameEntity* entity) const;

	void UpdateModificatorsVector();

	void UpdateTime();
	void ApplyWind(GameEntity* entity);

	// Heredado vía ISystem
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor & guest) override;
};

