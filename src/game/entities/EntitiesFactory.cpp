#include "stdafx.h"
#include "EntitiesFactory.h"
#include "../../NGenius.h"
#include "../../resources/Transformation.h"
#include "../../utils/Log.h"
#include "../ShooterGameConstants.h"
#include "../../resources/scene/GameScene.h"
#include "../../resources/GameEntity.h"
#include "../../resources/components/PhysicsComponent.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "../../resources/renderers/IRenderer.h"
#include "../../resources/entities/ParticlesEmitter.h"

#include "Battery.h"

EntitiesFactory::EntitiesFactory()
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

		entity = NGenius::GetInstance().CreateGameEntityFromModel("battery", transformation);
		entity->GetRenderer()->GetMaterial()->RemoveEffect<MaterialEffectDirectionalLightProperties>();

		ParticlesEmitter* steamParticlesEmitter = nullptr;// = mEngine->GetParticlesEmitter("steam");
		Battery* battery = new Battery(Item::ITEM_WATER_BATTERY, 100, *entity, steamParticlesEmitter);

		assert(battery != nullptr);

		scene->AddEntity(battery);
		if (steamParticlesEmitter != nullptr)
		{
			NGenius::GetInstance().AddParticleEmitter(steamParticlesEmitter);
			scene->AddEntity(steamParticlesEmitter);
		}
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