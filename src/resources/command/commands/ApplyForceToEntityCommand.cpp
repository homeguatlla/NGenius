#include "stdafx.h"
#include "ApplyForceToEntityCommand.h"
#include "src/resources/components/RigidbodyPhysicsComponent.h"
#include "src/resources/IGameEntity.h"

ApplyForceToEntityCommand::ApplyForceToEntityCommand(std::shared_ptr<IGameEntity> entity) :
	ICommand(entity)
{
}

void ApplyForceToEntityCommand::Execute()
{
	if (mEntity != nullptr)
	{
		PhysicsComponent* component = mEntity->GetComponent<PhysicsComponent>();
		auto rigidbodyComponent = static_cast<RigidbodyPhysicsComponent*>(component);
		rigidbodyComponent->AddForceAtBodyPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
		//rigidbodyComponent->AddForceAtBodyPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
	}
}

bool ApplyForceToEntityCommand::HasFinished() const
{
	return true;
}

void ApplyForceToEntityCommand::Update(float elapsedTime)
{
}
