#include "stdafx.h"
#include "PhysicsSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../GameEntity.h"
#include "../components/CollisionComponent.h"
#include "../components/EnergyWallCollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../entities/Terrain.h"
#include "../../renderer/IRenderer.h"


const glm::vec3 PhysicsSystem::GRAVITY_VALUE(0.0f, -9.8f, 0.0f);

PhysicsSystem::PhysicsSystem() :
mEnergyWallRadius(0.0f)
{
}


PhysicsSystem::~PhysicsSystem()
{
	mEntities.clear();
}

void PhysicsSystem::Update(float deltaTime)
{
	for (GameEntity* entity : mEntities)
	{
		ApplyMRU(deltaTime, entity);
		CheckCollisions(entity);		
	}
}

void PhysicsSystem::ApplyMRU(float deltaTime, GameEntity* entity)
{
	if (entity->HasComponent<PhysicsComponent>())
	{
		PhysicsComponent* physicsComponent = entity->GetComponent<PhysicsComponent>();

		Transformation* transformation = entity->GetTransformation();
		glm::vec3 position = transformation->GetPosition();

		const glm::vec3 gravity = physicsComponent->GetGravity();
		glm::vec3 velocity = physicsComponent->GetVelocity();

		velocity = velocity + gravity * deltaTime;
		position = position + velocity * deltaTime;

		transformation->SetPosition(position);
		physicsComponent->SetVelocity(velocity);
	}
}

void PhysicsSystem::CheckCollisions(GameEntity* entity)
{
	if (entity->HasComponent<CollisionComponent>())
	{
		CheckCollisionTerrain(entity);
	}
	if (entity->HasComponent<EnergyWallCollisionComponent>() && mEnergyWallRadius > 0.0f)
	{
		CheckCollisionEnergyWall(entity);
	}
}

void PhysicsSystem::CheckCollisionTerrain(GameEntity* entity)
{	
	CollisionComponent* collisionComponent = entity->GetComponent<CollisionComponent>();

	bool isInsideTerrain = IsInsideTerrain(entity);
	if (isInsideTerrain)
	{
		
		float groundHeight = 0.0f;
		bool isColliding = ApplyCollisions(entity, &groundHeight);
		collisionComponent->SetOnGround(isColliding);
		collisionComponent->SetGroundHeight(groundHeight);
		/*	//improvement, hasta el momento no tiene mucha utilidad, pero creo que debería ser útil.
		//cuando un objeto es estático si ha colisionado le eliminamos los componentes físicos de colisión y el físico para que no sea updateado
		//pues no va a colisionar más ni va a moverse más.
		if (collide && entity->HasComponent<PhysicsComponent>())
		{
		PhysicsComponent* physicComponent = entity->GetComponent<PhysicsComponent>();
		if (physicComponent->IsStatic())
		{
		entity->RemoveComponent<CollisionComponent>();
		entity->RemoveComponent<PhysicsComponent>();
		//TODO si no tiene más componentes quitarlo del physicsSystem
		}
		}*/
	}
	else
	{
		collisionComponent->SetOnGround(false);
	}
}

void PhysicsSystem::CheckCollisionEnergyWall(GameEntity* entity)
{
	EnergyWallCollisionComponent* collisionComponent = entity->GetComponent<EnergyWallCollisionComponent>();

	glm::vec3 collisionPoint;
	bool isColliding = ApplyEnergyWallCollision(entity, collisionPoint);

	collisionComponent->SetCollision(isColliding);
	collisionComponent->SetCollisionPoint(collisionPoint);
}

void PhysicsSystem::SetTerrain(const Terrain* terrain)
{
	assert(terrain != nullptr);

	mTerrain = terrain;
}

void PhysicsSystem::SetEnergyWallRadius(float radius)
{
	mEnergyWallRadius = radius;
}

void PhysicsSystem::AddEntity(GameEntity* entity)
{
	if (HasPhysicsComponents(entity))
	{
		mEntities.push_back(entity);
	}
}

void PhysicsSystem::RemoveEntity(GameEntity* entity)
{
	if (HasPhysicsComponents(entity))
	{
		std::vector<GameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		if (it != mEntities.end())
		{
			mEntities.erase(it);
		}
		else
		{
			assert(false);
		}
	}
}

bool PhysicsSystem::IsInsideTerrain(GameEntity *entity)
{
	Transformation* transformation = entity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();

	return mTerrain->IsPointInside(glm::vec2(position.x, position.z));
}

bool PhysicsSystem::HasPhysicsComponents(GameEntity* entity) const
{
	return entity != nullptr && (	entity->HasComponent<CollisionComponent>() ||
									entity->HasComponent<PhysicsComponent>() ||
									entity->HasComponent<EnergyWallCollisionComponent>()
								);
}

bool PhysicsSystem::ApplyCollisions(GameEntity *entity, float *groundHeight)
{
	Transformation* transformation = entity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();

	*groundHeight = mTerrain->GetHeight(glm::vec2(position.x, position.z));
	float entityBottomHeight = 0.0f;
	IRenderer* renderer = entity->GetRenderer();
	if (renderer != nullptr)
	{
		float rendererHeight = renderer->GetBoundingBox().GetVertexMax().y - renderer->GetBoundingBox().GetVertexMin().y;
		float factor = 1.0f/(rendererHeight * 100.0f);
		entityBottomHeight = glm::abs(renderer->GetBoundingBox().GetVertexMin().y * transformation->GetScale().y) + factor; 
	}
	position.y = glm::max(position.y, *groundHeight + entityBottomHeight);
	transformation->SetPosition(position);

	//is colliding
	return position.y <= *groundHeight + entityBottomHeight;
}

bool PhysicsSystem::ApplyEnergyWallCollision(GameEntity *entity, glm::vec3& collisionPoint)
{
	glm::vec3 energyWallCenter(0.0f);

	Transformation* transformation = entity->GetTransformation();

	float entityRadius = 0.0f;
	IRenderer* renderer = entity->GetRenderer();
	if (renderer != nullptr)
	{
		glm::vec3 side = renderer->GetBoundingBox().GetVertexMax() - renderer->GetBoundingBox().GetVertexMin();
		side *= transformation->GetScale();

		float maxSide = glm::max(side.x, glm::max(side.y, side.z));
		entityRadius = maxSide * 0.5f;
	}

	glm::vec3 position = transformation->GetPosition();
	glm::vec3 vector = position - energyWallCenter;

	float distanceToTheCenter = glm::distance(position, energyWallCenter);

	bool isInside = distanceToTheCenter < mEnergyWallRadius;
	bool isOutside = distanceToTheCenter > mEnergyWallRadius;
	bool isColliding = false;

	if (isInside)
	{
		isColliding = distanceToTheCenter > (mEnergyWallRadius - entityRadius);
		if (isColliding)
		{
			collisionPoint = glm::normalize(vector) * mEnergyWallRadius;
			position = energyWallCenter + glm::normalize(vector) * (mEnergyWallRadius - entityRadius);
			transformation->SetPosition(position);
		}
	}
	else if (isOutside)
	{
		isColliding = distanceToTheCenter <= (mEnergyWallRadius + entityRadius);
		if (isColliding)
		{
			collisionPoint = glm::normalize(vector) * mEnergyWallRadius;
			position = energyWallCenter + glm::normalize(vector) * (mEnergyWallRadius + entityRadius);
			transformation->SetPosition(position);
		}
	}

	//is colliding
	return isColliding;
}
