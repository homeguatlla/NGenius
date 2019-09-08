#include "stdafx.h"
#include "PhysicsComponent.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../renderers/IRenderer.h"
#include "../Memory.h"
#include "../systems/PhysicsSystem.h"


PhysicsComponent::PhysicsComponent(bool isStatic, float density, const glm::vec3& initialVelocity) :mIsStatic(isStatic), mDensity(density), mInitialVelocity(initialVelocity)
{
}

void PhysicsComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	CreatePhysicsData();
}

PhysicsComponent* PhysicsComponent::DoClone() const
{
	return DBG_NEW PhysicsComponent(*this);
}

bool PhysicsComponent::IsStatic() const
{
	return mIsStatic;
}

IComponent* PhysicsComponent::Create(IGameEntity* entity)
{
	PhysicsComponent* component = DBG_NEW PhysicsComponent();
	entity->AddComponent(component);

	return component;
}

void PhysicsComponent::SetVelocity(glm::vec3& velocity)
{
	mParticle->SetInitialVelocity(velocity);
}

//para calcular la densidad :
//V = volumen objecto = size.x * size.y * size.z
//liquid_density * V * 0.5 = F hacia arriba flotando en reposo
//calcular la gravedad P = m g  que es la fuerza hacia abajo
//Para equilibrio y flote estático P = F de donde puedes derivar la m
//calcular la densidad = m / V
//Luego, añadir el componente de drag con valores altos para estabilizar.

void PhysicsComponent::CreatePhysicsData()
{
	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	mParticle = std::make_shared<NPhysics::Particle>(position, mInitialVelocity);
	
	const AABB box = mParent->GetRenderer()->GetAABB();
	float volume = box.GetVolume();
	float mass = mDensity * volume;
	mParticle->SetMass(mass);
}

void PhysicsComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("is_static"), &mIsStatic);
	source->ReadParameter(std::string("density"), &mDensity);
	mInitialVelocity = glm::vec3(0.0f);
	source->BeginAttribute("initialVelocity");
	source->ReadParameter("X", &mInitialVelocity.x);
	source->ReadParameter("Y", &mInitialVelocity.y);
	source->ReadParameter("Z", &mInitialVelocity.z);
	source->EndAttribute();
}

void PhysicsComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("physics_component"));
	destination->WriteParameter(std::string("is_static"), mIsStatic);
	destination->WriteParameter(std::string("density"), mDensity);
	destination->WriteParameter(std::string("initialVelocity"), mInitialVelocity);
}