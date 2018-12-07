#include "stdafx.h"
#include "PhysicsSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../GameEntity.h"
#include "../components/CollisionComponent.h"
#include "../components/EnergyWallCollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../entities/Terrain.h"
#include "../renderers/IRenderer.h"

#include <iostream>
#include "../entities/Character.h"

const glm::vec3 PhysicsSystem::GRAVITY_VALUE(0.0f, -9.8f, 0.0f);

PhysicsSystem::PhysicsSystem() :
mEnergyWallRadius(0.0f),
mGravity(GRAVITY_VALUE)
{
}

PhysicsSystem::~PhysicsSystem()
{
	mEntities.clear();
}

unsigned int PhysicsSystem::GetNumberGameEntities() const
{
	return mEntities.size() + 1; //+1 terrain
}

const glm::vec3& PhysicsSystem::GetGravity() const
{
	return mGravity;
}

void PhysicsSystem::SetGravity(const glm::vec3& gravity)
{
	mGravity = gravity;
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

		//si tiene componente físico le bajamos la Y el coeficiente de penetración (introduction)
		PhysicsComponent* physicsComponent = entity->GetComponent<PhysicsComponent>();
		if (physicsComponent != nullptr)
		{
			Transformation* transformation = entity->GetTransformation();
			glm::vec3 position = transformation->GetPosition();
			position.y -= physicsComponent->GetIndroductionCoef();
			transformation->SetPosition(position);
		}
	
		collisionComponent->SetGroundHeight(groundHeight);
		
		//improvement, hasta el momento no tiene mucha utilidad, pero creo que debería ser útil.
		//cuando un objeto es estático si ha colisionado le eliminamos los componentes físicos de colisión y el físico para que no sea updateado
		//pues no va a colisionar más ni va a moverse más.
		/*if (isColliding && entity->HasComponent<PhysicsComponent>())
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

float PhysicsSystem::GetHeight(glm::vec2 point) const
{
	if (mTerrain != nullptr)
	{
		return mTerrain->GetHeight(point);
	}
	else
	{
		return 0.0f;
	}
}

void PhysicsSystem::SetEnergyWall(const glm::vec3& position, float radius)
{
	mEnergyWallRadius = radius;
	mEnergyWallPosition = position;
}



bool PhysicsSystem::IsInsideTerrain(GameEntity *entity)
{
	Transformation* transformation = entity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();

	return mTerrain->IsPointInside(glm::vec2(position.x, position.z));
}

bool PhysicsSystem::HasSpecificComponents(const GameEntity* entity) const
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
		glm::vec3 max = renderer->GetModelAABB().GetVertexMax();
		glm::vec3 min = renderer->GetModelAABB().GetVertexMin();
		glm::mat3 matrix = glm::mat3(transformation->GetModelMatrix());
		max = matrix * max;
		min = matrix * min;
		entityBottomHeight = glm::abs(min.y);
	}

	position.y = glm::max(position.y, *groundHeight + entityBottomHeight);
	transformation->SetPosition(position);

	//is colliding
	bool isColliding = position.y <= *groundHeight + entityBottomHeight;
	/*if (typeid(*entity) == typeid(Character))
	{
		std::cout << "position: " << position.y << " is colliding = " << isColliding << "\n";
	}*/
	return isColliding;
}

bool PhysicsSystem::ApplyEnergyWallCollision(GameEntity *entity, glm::vec3& collisionPoint)
{
	Transformation* transformation = entity->GetTransformation();

	float entityRadius = 0.0f;
	IRenderer* renderer = entity->GetRenderer();
	
	if (renderer != nullptr)
	{
		glm::vec3 side = renderer->GetAABB().GetVertexMax() - renderer->GetAABB().GetVertexMin();
		//side *= transformation->GetScale();

		float maxSide = glm::max(side.x, glm::max(side.y, side.z));
		entityRadius = maxSide * 0.5f;
	}

	glm::vec3 position = transformation->GetPosition();
	glm::vec3 vector = position - mEnergyWallPosition;

	float distanceToTheCenter = glm::length(vector);
	
	bool isInside = distanceToTheCenter < mEnergyWallRadius;
	bool isOutside = distanceToTheCenter >= mEnergyWallRadius;
	bool isColliding = false;

	if (isInside)
	{
		float diffRadius = mEnergyWallRadius - entityRadius;
		isColliding = distanceToTheCenter > diffRadius;
		if (isColliding)
		{
			collisionPoint = glm::normalize(vector) * mEnergyWallRadius;
			position = mEnergyWallPosition + glm::normalize(vector) * diffRadius;
			transformation->SetPosition(position);
		}
	}
	else if (isOutside)
	{
		float sumRadius = mEnergyWallRadius + entityRadius;
		isColliding = distanceToTheCenter <= sumRadius;
		if (isColliding)
		{
			collisionPoint = glm::normalize(vector) * mEnergyWallRadius;
			position = mEnergyWallPosition + glm::normalize(vector) * sumRadius;
			transformation->SetPosition(position);
		}
	}

	//is colliding
	return isColliding;
}