#include "stdafx.h"
#include "EntitiesFactory.h"
#include "../../NGenius.h"
#include "../../resources/Transformation.h"
#include "../../utils/Log.h"
#include "../ShooterGameConstants.h"
#include "../../resources/GameEntity.h"
#include "../../resources/components/PhysicsComponent.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "../../resources/renderers/IRenderer.h"

#include "Battery.h"

EntitiesFactory::EntitiesFactory(NGenius* engine) : mEngine(engine)
{
}


EntitiesFactory::~EntitiesFactory()
{
}


GameEntity* EntitiesFactory::Create(Item::ItemType type, const glm::vec3& position, GameScene* scene)
{
	GameEntity* entity = nullptr;

	switch (type)
	{
	case Item::ITEM_SHOT_GUN:
		break;
	case Item::ITEM_WATER_BATTERY:
	{
		Transformation* transformation = new Transformation(position, glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.0003f));

		entity = mEngine->CreateGameEntityFromModel("battery", transformation);
		PhysicsComponent* component = entity->GetComponent<PhysicsComponent>();
		component->SetVelocity(MARS_GRAVITY_VALUE);

		entity->GetRenderer()->GetMaterial()->RemoveEffect<MaterialEffectDirectionalLightProperties>();
		Battery* battery = new Battery(Item::ITEM_WATER_BATTERY, 100, *entity);

		battery->AddIntoScene(scene);
		entity = battery;
		break;
	}
	case Item::ITEM_INVALID:
	default:
		Log(Log::LOG_ERROR) << "Error, unknown item type! \n";
		break;
	}

	return entity;
}