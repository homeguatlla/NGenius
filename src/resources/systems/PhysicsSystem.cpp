#include "stdafx.h"
#include "PhysicsSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "../IGameEntity.h"

#include "../components/CollisionComponent.h"
#include "../components/EnergyWallCollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/ParticlePhysicsComponent.h"
#include "../components/RigidbodyPhysicsComponent.h"
#include "../components/GravityComponent.h"
#include "../components/BuoyancyComponent.h"
#include "../components/DragComponent.h"

#include "../../NPhysics/source/particle/forceGenerators/ParticleDrag.h"
#include "../../NPhysics/source/particle/forceGenerators/ParticleBuoyancy.h"
#include "../../NPhysics/source/particle/forceGenerators/ParticleGravity.h"

#include "../../NPhysics/source/rigidbody/forceGenerators/RigidBodyDrag.h"
#include "../../NPhysics/source/rigidbody/forceGenerators/RigidBodyBuoyancy.h"
#include "../../NPhysics/source/rigidbody/forceGenerators/RigidBodyGravity.h"

#include "../entities/Terrain.h"
#include "../renderers/IRenderer.h"



#include <iostream>
#include "../entities/Player.h"

const glm::vec3 PhysicsSystem::GRAVITY_VALUE(0.0f, -9.8f, 0.0f);

PhysicsSystem::PhysicsSystem() :
mEnergyWallRadius(0.0f)
{
}

PhysicsSystem::~PhysicsSystem()
{
	Release();
}

void PhysicsSystem::Release()
{
	mEntities.clear();
}

unsigned int PhysicsSystem::GetNumberGameEntities() const
{
	return mEntities.size() + 1; //+1 terrain
}

void PhysicsSystem::Update(float deltaTime)
{
	for (IGameEntity* entity : mEntities)
	{
		UpdatePhysicsObjectsData(entity);
	}

	mEngine.Update(deltaTime);

	for (IGameEntity* entity : mEntities)
	{
		UpdateEntitiesData(entity);
		CheckCollisions(entity);
	}
}

void PhysicsSystem::Reload()
{
	Release();
}

void PhysicsSystem::CheckCollisions(IGameEntity* entity)
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

void PhysicsSystem::CheckCollisionTerrain(IGameEntity* entity)
{	
	CollisionComponent* collisionComponent = entity->GetComponent<CollisionComponent>();

	bool isInsideTerrain = IsInsideTerrain(entity);
	if (isInsideTerrain)
	{
		
		float groundHeight = 0.0f;
		bool isColliding = ApplyCollisions(entity, &groundHeight);
		collisionComponent->SetOnGround(isColliding);
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

void PhysicsSystem::CheckCollisionEnergyWall(IGameEntity* entity)
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

void PhysicsSystem::SetEnergyWall(const glm::vec3& position, float radius)
{
	mEnergyWallRadius = radius;
	mEnergyWallPosition = position;
}

void PhysicsSystem::AddEntity(IGameEntity* entity)
{
	mEntities.push_back(entity);
}

void PhysicsSystem::RemoveEntity(IGameEntity* entity)
{
	if (HasPhysicsComponents(entity))
	{
		std::vector<IGameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](IGameEntity* a) { return a == entity; });
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

bool PhysicsSystem::IsInsideTerrain(IGameEntity*entity)
{
	Transformation* transformation = entity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();

	return mTerrain->IsPointInside(glm::vec2(position.x, position.z));
}

bool PhysicsSystem::HasPhysicsComponents(const IGameEntity* entity) const
{
	return entity != nullptr && (	entity->HasComponent<CollisionComponent>() ||
									entity->HasComponent<PhysicsComponent>() ||
									entity->HasComponent<EnergyWallCollisionComponent>()
								);
}

void PhysicsSystem::OnGameEntityAdded(IGameEntity* entity)
{
	if (HasPhysicsComponents(entity))
	{
		AddEntity(entity);
		PhysicsComponent* component = entity->GetComponent<PhysicsComponent>();
		
		if (component != nullptr && typeid(*component) == typeid(ParticlePhysicsComponent)) 
		{
			auto particle = std::static_pointer_cast<NPhysics::Particle>(component->GetPhysicsObject());
			mEngine.AddParticle(particle);
			AddGenerators(particle, entity);
		}
		else if(component != nullptr && typeid(*component) == typeid(RigidbodyPhysicsComponent))
		{
			auto body = std::static_pointer_cast<NPhysics::RigidBody>(component->GetPhysicsObject());
			auto volume = component->GetPhysicsBouningVolume();
			mEngine.AddRigidBody(body, volume);
			AddGenerators(body, volume, entity);
		}
	}

	//TODO esto no funcionará así, pero por ahora sí. 
	if (typeid(*entity) == typeid(Terrain))
	{
		SetTerrain(static_cast<Terrain*>(entity));
	}
}

void PhysicsSystem::AddGenerators(std::shared_ptr<NPhysics::Particle>& particle, IGameEntity* entity)
{
	GravityComponent* gravityComponent = entity->GetComponent<GravityComponent>();
	if (gravityComponent != nullptr)
	{
		std::shared_ptr<NPhysics::IForceGenerator<NPhysics::Particle>> generator;
		generator = std::make_shared<NPhysics::ParticleGravity>(gravityComponent->GetGravity());
		mEngine.RegisterParticleForceGenerator(particle, generator);
	}

	BuoyancyComponent* buoyancyComponent = entity->GetComponent<BuoyancyComponent>();
	if (buoyancyComponent != nullptr)
	{
		std::shared_ptr<NPhysics::IForceGenerator<NPhysics::Particle>> generator;
		generator = std::make_shared<NPhysics::ParticleBuoyancy>(
			buoyancyComponent->GetMaxDepth(), 
			buoyancyComponent->GetVolume(), 
			buoyancyComponent->GetWaterHeight(), 
			buoyancyComponent->GetLiquidDensity());
		mEngine.RegisterParticleForceGenerator(particle, generator);
	}

	DragComponent* dragComponent = entity->GetComponent<DragComponent>();
	if (dragComponent != nullptr)
	{
		std::shared_ptr<NPhysics::IForceGenerator<NPhysics::Particle>> generator;
		generator = std::make_shared<NPhysics::ParticleDrag>(dragComponent->GetK1(), dragComponent->GetK2());
		mEngine.RegisterParticleForceGenerator(particle, generator);
	}
}

void PhysicsSystem::AddGenerators(std::shared_ptr<NPhysics::RigidBody>& rigidBody, std::shared_ptr<NPhysics::IBoundingVolume>& volume, IGameEntity* entity)
{
	GravityComponent* gravityComponent = entity->GetComponent<GravityComponent>();
	if (gravityComponent != nullptr)
	{
		std::shared_ptr<NPhysics::IForceGenerator<NPhysics::RigidBody>> generator;
		generator = std::make_shared<NPhysics::RigidBodyGravity>(gravityComponent->GetGravity());
		mEngine.RegisterRigidBodyForceGenerator(rigidBody, volume, generator);
	}

	BuoyancyComponent* buoyancyComponent = entity->GetComponent<BuoyancyComponent>();
	if (buoyancyComponent != nullptr)
	{
		std::shared_ptr<NPhysics::IForceGenerator<NPhysics::RigidBody>> generator;
		generator = std::make_shared<NPhysics::RigidBodyBuoyancy>(
			buoyancyComponent->GetMaxDepth(),
			buoyancyComponent->GetVolume(),
			buoyancyComponent->GetWaterHeight(),
			buoyancyComponent->GetLiquidDensity(),
			buoyancyComponent->GetCenter());
		mEngine.RegisterRigidBodyForceGenerator(rigidBody, volume, generator);
	}

	DragComponent* dragComponent = entity->GetComponent<DragComponent>();
	if (dragComponent != nullptr)
	{
		std::shared_ptr<NPhysics::IForceGenerator<NPhysics::RigidBody>> generator;
		generator = std::make_shared<NPhysics::RigidBodyDrag>(dragComponent->GetK1(), dragComponent->GetK2());
		mEngine.RegisterRigidBodyForceGenerator(rigidBody, volume, generator);
	}
}

void PhysicsSystem::OnGameEntityRemoved(IGameEntity* entity)
{
	if (HasPhysicsComponents(entity))
	{
		RemoveEntity(entity);
	}
}

bool PhysicsSystem::ApplyCollisions(IGameEntity*entity, float *groundHeight)
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
	/*if (typeid(*entity) == typeid(Player))
	{
		std::cout << "position: " << position.y << " is colliding = " << isColliding << "\n";
	}*/
	return isColliding;
}

bool PhysicsSystem::ApplyEnergyWallCollision(IGameEntity*entity, glm::vec3& collisionPoint)
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

void PhysicsSystem::UpdatePhysicsObjectsData(IGameEntity* entity)
{
	PhysicsComponent* component = entity->GetComponent<PhysicsComponent>();
	if (component != nullptr)
	{
		auto object = component->GetPhysicsObject();
		auto position = entity->GetTransformation()->GetPosition();
		auto rotation = entity->GetTransformation()->GetRotation();
		object->SetPosition(position);
		object->SetRotation(rotation);
	}
}

void PhysicsSystem::UpdateEntitiesData(IGameEntity* entity)
{
	PhysicsComponent* component = entity->GetComponent<PhysicsComponent>();
	if (component != nullptr)
	{
		auto object = component->GetPhysicsObject();
		entity->GetTransformation()->SetPosition(object->GetPosition());
		glm::vec3 rotation = object->GetRotation();
		//std::cout << "euler angles: " << rotation.x << ", " << rotation.y << ", " << rotation.z << "\n";

		entity->GetTransformation()->SetRotation(object->GetRotation());
	}
}

BaseVisitable<>::ReturnType PhysicsSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}