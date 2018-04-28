#include "stdafx.h"
#include "ParticlesEmitter.h"
#include "Particle.h"
#include "../components/PhysicsComponent.h"
#include "../components/CollisionComponent.h"
#include "../systems/PhysicsSystem.h"
#include "../systems/ParticlesSystem.h"
#include "../textures/Texture.h"
#include "../shaders/IShaderProgram.h"
#include "../scene/GameScene.h"

#include <iostream>

ParticlesEmitter::ParticlesEmitter(Particle* particle, Transformation* transformation, IRenderer* renderer, float spawnRate) :
GameEntity(transformation, renderer),
mOriginalParticle(particle),
mSpawnRate(spawnRate),
mParticlesToSpawn(0.0f),
mColorOrigin(glm::vec4(1.0f)),
mColorDestination(glm::vec4(1.0f)),
mScale(glm::vec2(1.0f)),
mVelocityMin(0.0f),
mVelocityMax(0.0f),
mSpawnAreaMin(0.0f),
mSpawnAreaMax(0.0f),
mRotationSpeedMin(0.0f),
mRotationSpeedMax(0.0f),
mGameScene(nullptr),
mMaxParticlesUpdate(MAX_PARTICLES)
{
	mParticles.reserve(MAX_PARTICLES);
	for (unsigned int i = 0; i < MAX_PARTICLES; ++i)
	{
		mParticlesPool.push_back(static_cast<Particle*>(mOriginalParticle->Clone()));
	}
}

void ParticlesEmitter::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);
	if (IsEnabled())
	{
		RemoveDeadParticles();
		SpawnNewParticles(elapsedTime);
		/*
		for (int i = 0; i < mParticles.size(); ++i)
		{
			std::cout << "particle: " << i << " live: " << mParticles[i]->GetLiveTime() << "\n";
		}*/
	}
}

ParticlesEmitter::~ParticlesEmitter()
{
	for (unsigned long i = 0; i < mNewParticlesToBeAdded.size(); ++i)
	{
		delete mNewParticlesToBeAdded[i];
	}

	mNewParticlesToBeAdded.clear();
	mParticles.clear();
}

void ParticlesEmitter::RemoveDeadParticles()
{
	ParticlesIterator iterator;

	for (unsigned long i = 0; i < mParticles.size();)
	{
		if (!mParticles[i]->IsAlive())
		{
			RemoveParticle(i);
		}
		else
		{
			++i;
		}
	}
}

void ParticlesEmitter::SpawnNewParticles(float elapsedTime)
{
	assert(mGameScene != nullptr);

	mParticlesToSpawn += mSpawnRate * elapsedTime;

	int maxParticlesCanCreate = MAX_PARTICLES - GetNumberParticles();
	int particles = glm::min(maxParticlesCanCreate, glm::min(static_cast<int>(mParticlesToSpawn), mMaxParticlesUpdate));
	
	for (int i = 0; i < particles; ++i)
	{
		Particle* particle = CreateParticle();
		mParticles.push_back(particle);
		mGameScene->AddEntity(particle);
	}

	mParticlesToSpawn -= particles;
}

unsigned int ParticlesEmitter::GetNumberParticles() const
{
	return mParticles.size() + mNewParticlesToBeAdded.size();
}

void ParticlesEmitter::SetMaxParticlesUpdate(int maxParticlesUpdate)
{
	mMaxParticlesUpdate = maxParticlesUpdate;
}

bool ParticlesEmitter::CanCreateParticle() const
{
	return mParticles.size() + mNewParticlesToBeAdded.size() < MAX_PARTICLES;
}

void ParticlesEmitter::RemoveParticle(unsigned long index)
{
	assert(mGameScene != nullptr);
	
	mGameScene->RemoveEntity(mParticles[index]);
	mParticlesPool.push_back(mParticles[index]);
	std::swap(mParticles[index], mParticles.back());
	mParticles.pop_back();
}

Particle* ParticlesEmitter::CreateParticle()
{
	assert(mParticles.size() + mParticlesPool.size() == MAX_PARTICLES);

	Particle* particle = mParticlesPool.front();
	mParticlesPool.pop_front();

	//POSITION
	glm::vec3 position = GetTransformation()->GetPosition();
	float lambda = static_cast<float>(rand() % 100) / 100.0f;
	position.x += mSpawnAreaMin.x + lambda * (mSpawnAreaMax.x - mSpawnAreaMin.x);
	lambda = static_cast<float>(rand() % 100) / 100.0f;
	position.y += mSpawnAreaMin.y + lambda * (mSpawnAreaMax.y - mSpawnAreaMin.y);
	lambda = static_cast<float>(rand() % 100) / 100.0f;
	position.z += mSpawnAreaMin.x + lambda * (mSpawnAreaMax.z - mSpawnAreaMin.z);
	
	particle->GetTransformation()->SetPosition(position);
	
	//LIVE
	particle->SetLiveTime(particle->GetMaxLiveTime());
	
	//ROTATION
	int rotationRange = static_cast<int>(mRotationSpeedMax - mRotationSpeedMin);
	if (abs(rotationRange) > 0)
	{
		float sign = 1.0f - 2.0f * (rand() % 2);
		float speed = static_cast<float>(rand() % rotationRange);
		particle->SetRotationSpeed(sign * speed);
	}

	//SCALE
	particle->SetScaleFactor(mScale);
	
	//COLOR
	particle->SetColorGradientValues(mColorOrigin, mColorDestination);
	
	//VELOCITY
	if (particle->HasComponent<PhysicsComponent>())
	{
		lambda = static_cast<float>(rand() % 100) / 100.0f;
		float xVelocity = mVelocityMin.x + lambda * (mVelocityMax.x - mVelocityMin.x);
		lambda = static_cast<float>(rand() % 100) / 100.0f;
		float zVelocity = mVelocityMin.z + lambda * (mVelocityMax.z - mVelocityMin.z);
		lambda = static_cast<float>(rand() % 100) / 100.0f;
		float yVelocity = mVelocityMin.y + lambda * (mVelocityMax.y - mVelocityMin.y);
		PhysicsComponent* physicsComponent = particle->GetComponent<PhysicsComponent>();
		physicsComponent->SetVelocity(glm::vec3(xVelocity, yVelocity, zVelocity));
	}
	return particle;
}

void ParticlesEmitter::SetGameScene(GameScene* gameScene)
{
	mGameScene = gameScene;
}

void ParticlesEmitter::SetColorGradientValues(const glm::vec4& origin, const glm::vec4& destination)
{
	mColorOrigin = origin;
	mColorDestination = destination;
}

void ParticlesEmitter::SetScaleValues(float origin, float destination)
{
	mScale.x = origin;
	mScale.y = destination;
}

void ParticlesEmitter::SetVelocity(const glm::vec3& min, const glm::vec3& max)
{
	mVelocityMin = min;
	mVelocityMax = max;
}

void ParticlesEmitter::SetSpawnArea(const glm::vec3& min, const glm::vec3& max)
{
	mSpawnAreaMin = min;
	mSpawnAreaMax = max;
}

void ParticlesEmitter::SetRotationSpeed(float min, float max)
{
	mRotationSpeedMin = min;
	mRotationSpeedMax = max;
}