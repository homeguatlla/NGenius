#include "stdafx.h"
#include "PhysicsSystem.h"
#include <GLFW/glfw3.h>
#include <algorithm>

#include "src/resources//IGameEntity.h"

#include "src/resources/components/CollisionComponent.h"
#include "src/resources/components/EnergyWallCollisionComponent.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/ParticlePhysicsComponent.h"
#include "src/resources/components/RigidbodyPhysicsComponent.h"
#include "src/resources/components/GravityComponent.h"
#include "src/resources/components/BuoyancyComponent.h"
#include "src/resources/components/DragComponent.h"

#include "source/particle/forceGenerators/ParticleDrag.h"
#include "source/particle/forceGenerators/ParticleBuoyancy.h"
#include "source/particle/forceGenerators/ParticleGravity.h"
#include "source/rigidbody/forceGenerators/RigidBodyDrag.h"
#include "source/rigidbody/forceGenerators/RigidBodyBuoyancy.h"
#include "source/rigidbody/forceGenerators/RigidBodyGravity.h"

#include "src/resources/entities/Terrain.h"
#include "src/resources/entities/Player.h"
#include "src/resources/renderers/IRenderer.h"
#include "source/utils/Math.h"

#include <iostream>

const glm::vec3 PhysicsSystem::GRAVITY_VALUE(0.0f, -9.8f, 0.0f);

PhysicsSystem::PhysicsSystem() :
	mEnergyWallRadius(0.0f),
	mCanUpdate(true)
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

void PhysicsSystem::EnableUpdate(bool enable)
{
	mCanUpdate = enable;
}

void PhysicsSystem::Update(float deltaTime)
{
	if (mCanUpdate)
	{
		for (IGameEntity* entity : mEntities)
		{
			UpdatePhysicsObjectsData(entity);
		}

		mEngine.Update(deltaTime);

		for (IGameEntity* entity : mEntities)
		{
			CheckCollisions(entity);
			UpdateEntitiesData(entity);
		}
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
		//CheckCollisionEnergyWall(entity);
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
		
		//improvement, hasta el momento no tiene mucha utilidad, pero creo que deber�a ser �til.
		//cuando un objeto es est�tico si ha colisionado le eliminamos los componentes f�sicos de colisi�n y el f�sico para que no sea updateado
		//pues no va a colisionar m�s ni va a moverse m�s.
		/*if (isColliding && entity->HasComponent<PhysicsComponent>())
		{
			PhysicsComponent* physicComponent = entity->GetComponent<PhysicsComponent>();
			if (physicComponent->IsStatic())
			{
				entity->RemoveComponent<CollisionComponent>();
				entity->RemoveComponent<PhysicsComponent>();
				//TODO si no tiene m�s componentes quitarlo del physicsSystem
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
	auto physicsComponent = entity->GetComponent<PhysicsComponent>();
	if (physicsComponent != nullptr)
	{
		//Transformation* transformation = entity->GetTransformation();
		//glm::vec3 position = transformation->GetPosition();

		auto position = physicsComponent->GetPhysicsObject()->GetPosition();

		return mTerrain->IsPointInside(glm::vec2(position.x, position.z));
	}

	return true;
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
			auto volume = component->GetPhysicsBoundingVolume();
			mEngine.AddRigidBody(body, volume);
			AddGenerators(body, volume, entity);
		}
	}

	//TODO esto no funcionar� as�, pero por ahora s�. 
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
	/*Transformation* transformation = entity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();
	*/
	PhysicsComponent* component = entity->GetComponent<PhysicsComponent>();
	if (component != nullptr)
	{
		auto physicsObject = component->GetPhysicsObject();
		if (physicsObject->GetType() != NPhysics::PhysicsType::kStatic)
		{
			auto boundingVolume = component->GetPhysicsBoundingVolume();
			auto position = boundingVolume->GetPosition();

			*groundHeight = mTerrain->GetHeight(glm::vec2(position.x, position.z));
			float newY = glm::max(position.y, *groundHeight + boundingVolume->GetSize().y * 0.5f);
			bool isColliding = position.y < newY;

			if (isColliding)
			{
				position.y = newY;
				boundingVolume->SetPosition(position);
				physicsObject->SetPosition(position);
				//TODO esto se est� cargando la fisica real. Habr�a que hacerlo dentro del motor de fisica.
				physicsObject->SetInitialVelocity(glm::vec3(0.0f));
				physicsObject->SetAcceleration(glm::vec3(0.0f));
			}
			
			/*bool isColliding = (position.y < *groundHeight + entityBottomHeight) || 
				(NPhysics::NMath::IsNearlyEqual(position.y, *groundHeight + entityBottomHeight, EPSILON2));
			*/
			return isColliding;
		}
	}
	
	return false;
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
		//as we are setting to the physicsObject the position of the gameEntity
		//because if is the player, position could be changed, we have to set the game entity position
		//adding the translation the physics object has related the game entity.
		object->SetPosition(position + component->GetTranslation());
		object->SetRotation(rotation);

		auto boundingVolume = component->GetPhysicsBoundingVolume();
		boundingVolume->SetPosition(position + component->GetTranslation());
	}
}

void PhysicsSystem::UpdateEntitiesData(IGameEntity* entity)
{
	PhysicsComponent* component = entity->GetComponent<PhysicsComponent>();
	if (component != nullptr)
	{
		auto object = component->GetPhysicsObject();
		entity->GetTransformation()->SetPosition(object->GetPosition() - component->GetTranslation());
		glm::vec3 rotation = object->GetRotation();
		//std::cout << "euler angles: " << rotation.x << ", " << rotation.y << ", " << rotation.z << "\n";

		entity->GetTransformation()->SetRotation(object->GetRotation());
	}
}

BaseVisitable<>::ReturnType PhysicsSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}