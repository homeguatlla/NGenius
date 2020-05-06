#include "stdafx.h"
#include "PhysicsComponent.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/Transformation.h"
#include "src/utils/serializer/XMLSerializer.h"
#include "src/utils/serializer/XMLDeserializer.h"
#include "src/resources/renderers/IRenderer.h"
#include "Memory.h"
#include "src/resources/systems/PhysicsSystem.h"
#include "source/InstantiableObject.h"
#include "source/bvh/boundingVolumes/SphereBoundingVolume.h"
#include "source/bvh/boundingVolumes/BoxBoundingVolume.h"
#include <memory>


PhysicsComponent::PhysicsComponent() :
	mType(NPhysics::PhysicsType::kStatic),
	mInitialVelocity(0.0f),
	mLocalTranslation(0.0f),
	mLocalScale(1.0f),
	mLocalRotation(0.0f),
	mDensity(1.0f),
	mRestitution(1.0f),
	mSize(0.0f),
	mBoundingVolume{ nullptr }
{
}

PhysicsComponent::PhysicsComponent(NPhysics::PhysicsType type, float density, const glm::vec3& initialVelocity) :
	mType(type), 
	mInitialVelocity(initialVelocity),
	mLocalTranslation(0.0f),
	mLocalScale(1.0f),
	mLocalRotation(0.0f),
	mDensity(density),
	mRestitution(1.0f),
	mSize(0.0f),
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

std::shared_ptr<NPhysics::IBoundingVolume> PhysicsComponent::GetPhysicsBoundingVolume() const
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
	source->ReadParameter(std::string("restitution"), &mRestitution);
	
	if (source->HasAttribute("initialVelocity"))
	{
		source->BeginAttribute("initialVelocity");
		source->ReadParameter("X", &mInitialVelocity.x);
		source->ReadParameter("Y", &mInitialVelocity.y);
		source->ReadParameter("Z", &mInitialVelocity.z);
		source->EndAttribute();
	}

	if (source->HasAttribute("translation"))
	{
		source->BeginAttribute("translation");
		source->ReadParameter("X", &mLocalTranslation.x);
		source->ReadParameter("Y", &mLocalTranslation.y);
		source->ReadParameter("Z", &mLocalTranslation.z);
		source->EndAttribute();
	}
	
	if (source->HasAttribute("scale"))
	{
		source->BeginAttribute("scale");
		source->ReadParameter("X", &mLocalScale.x);
		source->ReadParameter("Y", &mLocalScale.y);
		source->ReadParameter("Z", &mLocalScale.z);
		source->EndAttribute();
	}

	if (source->HasAttribute("rotation"))
	{
		source->BeginAttribute("rotation");
		source->ReadParameter("X", &mLocalRotation.x);
		source->ReadParameter("Y", &mLocalRotation.y);
		source->ReadParameter("Z", &mLocalRotation.z);
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
		mBoundingVolume = NPhysics::InstantiableObject::CreateBoundingVolume(
			boundingVolumeType, mParent->GetTransformation()->GetModelMatrix());

		if (source->HasAttribute("size"))
		{
			source->BeginAttribute("size");
			source->ReadParameter("X", &mSize.x);
			source->ReadParameter("Y", &mSize.y);
			source->ReadParameter("Z", &mSize.z);
			source->EndAttribute();
		}
		source->EndAttribute();
	}
}

void PhysicsComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("physics_component"));
	destination->WriteParameter(std::string("physics_type"), static_cast<int>(mType), "");
	destination->WriteParameter(std::string("density"), mDensity);
	destination->WriteParameter(std::string("restitution"), mRestitution);
	destination->WriteParameter(std::string("initialVelocity"), mInitialVelocity);
}