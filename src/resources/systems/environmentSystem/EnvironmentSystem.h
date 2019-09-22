#pragma once
#include <glm/glm.hpp>
#include "../../../visitor/BaseVisitable.h"
#include "../../scene/IGameSceneListener.h"
#include <vector>
#include <memory>
#include <string>

class IGameEntity;
class Terrain;
class SunLight;

class EnvironmentSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<IGameEntity*> mEntities;
	std::vector<IGameEntity*> mModificators;
	const Terrain* mTerrain;
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

	void SetTerrain(const Terrain* terrain);
	
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
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);
	void RemoveEntityVector(IGameEntity* entity, std::vector<IGameEntity*>& vector);
	bool HasEnvironmentComponents(const IGameEntity* entity) const;

	void UpdateModificatorsVector();

	void UpdateTime();
	void ApplyWind(IGameEntity* entity);

	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;
};

