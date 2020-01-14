#include "stdafx.h"
#include "ApplyForceToEntityCommand.h"
#include "../../components/RigidbodyPhysicsComponent.h"
#include "../../IGameEntity.h"

ApplyForceToEntityCommand::ApplyForceToEntityCommand(IGameEntity* entity) :
	ICommand(entity)
{
}


ApplyForceToEntityCommand::~ApplyForceToEntityCommand()
{
}

void ApplyForceToEntityCommand::Execute()
{
	if (mEntity != nullptr)
	{
		PhysicsComponent* component = mEntity->GetComponent<PhysicsComponent>();
		auto rigidbodyComponent = static_cast<RigidbodyPhysicsComponent*>(component);
		rigidbodyComponent->AddForceAtBodyPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(-0.5f, 1.0f, 0.0f));
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
