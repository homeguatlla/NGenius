#pragma once
#include "../BaseGameEntity.h"
#include <vector>
#include <list>

class Texture;
class IShaderProgram;
class Particle;
class GameScene;

class ParticlesEmitter : public BaseGameEntity<ParticlesEmitter>
{
	std::list<Particle*> mParticlesPool;
	std::vector<Particle*> mParticles;
	std::vector<Particle*> mNewParticlesToBeAdded;
	typedef std::vector<Particle*>::iterator ParticlesIterator;
	Particle* mOriginalParticle;
	const unsigned long MAX_PARTICLES = 512;
	float mSpawnRate;
	float mParticlesToSpawn;
	glm::vec4 mColorOrigin;
	glm::vec4 mColorDestination;
	glm::vec2 mScale;

	glm::vec3 mVelocityMin;
	glm::vec3 mVelocityMax;

	glm::vec3 mSpawnAreaMin;
	glm::vec3 mSpawnAreaMax;

	float mRotationSpeedMax;
	float mRotationSpeedMin;

	int mMaxParticlesUpdate;

	//from global
	GameScene* mGameScene;

public:
	ParticlesEmitter(Particle* particle, Transformation* transformation, IRenderer* renderer, float spawnRate);
	~ParticlesEmitter();

	ParticlesEmitter* DoClone() const override { return nullptr; }

	void SetGameScene(GameScene* gameScene);

	void Update(float elapsedTime) override;
	void SetColorGradientValues(const glm::vec4& origin, const glm::vec4& destination);
	void SetScaleValues(float origin, float destination);
	void SetVelocity(const glm::vec3& min, const glm::vec3& max);
	void SetRotationSpeed(float min, float max);
	void SetSpawnArea(const glm::vec3& min, const glm::vec3& max);
	unsigned int GetNumberParticles() const;
	void SetMaxParticlesUpdate(int maxParticlesUpdate);

private:
	bool CanCreateParticle() const;
	Particle* CreateParticle();
	void RemoveParticle(unsigned long index);
	void SpawnNewParticles(float elapsedTime);
	void RemoveDeadParticles();
};