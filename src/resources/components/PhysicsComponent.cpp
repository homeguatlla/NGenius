#include "stdafx.h"
#include "PhysicsComponent.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../renderers/IRenderer.h"
#include "../Memory.h"
#include "../systems/PhysicsSystem.h"
#include "../../NPhysics/source/InstantiableObject.h"
#include "../../NPhysics/source/bvh/boundingVolumes/SphereBoundingVolume.h"


PhysicsComponent::PhysicsComponent(NPhysics::PhysicsType type, float density, const glm::vec3& initialVelocity) :
	mType(type), 
	mDensity(density), 
	mInitialVelocity(initialVelocity),
	mBoundingVolume { nullptr }
{
}

void PhysicsComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	//para calcular la densidad :
	//V = volumen objecto = size.x * size.y * size.z
	//liquid_density * V * 0.5 = F hacia arriba flotando en reposo
	//calcular la gravedad P = m g  que es la fuerza hacia abajo
	//Para equilibrio y flote estático P = F de donde puedes derivar la m
	//calcular la densidad = m / V
	//Luego, añadir el componente de drag con valores altos para estabilizar.

	DoCreatePhysicsData();
	assert(mObject);
}

NPhysics::PhysicsType PhysicsComponent::GetType() const
{
	return mType;
}

const glm::vec3 PhysicsComponent::GetVelocity() const
{
	return mObject->GetVelocity();
}

void PhysicsComponent::SetInitialVelocity(const glm::vec3& velocity)
{
	mInitialVelocity = velocity;
}

void PhysicsComponent::SetVelocity(const glm::vec3& velocity)
{
	mObject->SetInitialVelocity(velocity);
}

std::shared_ptr<NPhysics::PhysicsObject> PhysicsComponent::GetPhysicsObject() const
{
	return mObject;
}

std::shared_ptr<NPhysics::IBoundingVolume> PhysicsComponent::GetPhysicsBouningVolume() const
{
	return mBoundingVolume;
}

void PhysicsComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	int type = 0;
	source->ReadParameter(std::string("physics_type"), &type);

	//TODO esto hay que mirar de hacer una funcion que te transforme, y validar que sea un tipo valido
	mType = static_cast<NPhysics::PhysicsType>(type);

	source->ReadParameter(std::string("density"), &mDensity);
	mInitialVelocity = glm::vec3(0.0f);
	if (source->HasAttribute("initialVelocity"))
	{
		source->BeginAttribute("initialVelocity");
		source->ReadParameter("X", &mInitialVelocity.x);
		source->ReadParameter("Y", &mInitialVelocity.y);
		source->ReadParameter("Z", &mInitialVelocity.z);
		source->EndAttribute();
	}

	ReadBoundingVolumeFrom(source);
}

void PhysicsComponent::ReadBoundingVolumeFrom(core::utils::IDeserializer* source)
{
	if (source->HasAttribute("collider"))
	{
		source->BeginAttribute("collider");

		std::string boundingVolumeType;
		source->ReadParameter("type", boundingVolumeType);
		mBoundingVolume = NPhysics::InstantiableObject::CreateBoundingVolume(boundingVolumeType); 
		if (boundingVolumeType == NPhysics::SphereBoundingVolume::GetClassName())
		{
			auto sphereVolume = std::dynamic_pointer_cast<NPhysics::SphereBoundingVolume>(mBoundingVolume);
			float radius = 0.0f;
			source->ReadParameter("radius", &radius);
			sphereVolume->SetRadius(radius);
		}
		else
		{
			assert(false);
		}
		source->EndAttribute();
	}
}

void PhysicsComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("physics_component"));
	destination->WriteParameter(std::string("physics_type"), static_cast<int>(mType), "");
	destination->WriteParameter(std::string("density"), mDensity);
	destination->WriteParameter(std::string("initialVelocity"), mInitialVelocity);
}