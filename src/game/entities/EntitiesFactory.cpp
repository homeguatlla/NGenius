#include "stdafx.h"
#include "EntitiesFactory.h"
#include "../ShooterGameConstants.h"
#include "Battery.h"

#include "../../NGenius.h"
#include "../../resources/Transformation.h"
#include "../../utils/Log.h"

#include "../../resources/scene/GameScene.h"

#include "../../resources/GameEntity.h"
#include "../../resources/entities/ParticlesEmitter.h"

#include "../../resources/components/PhysicsComponent.h"
#include "../../resources/components/AttachGameEntityComponent.h"

#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"

#include "../../resources/renderers/IRenderer.h"


EntitiesFactory::EntitiesFactory()
{
}


EntitiesFactory::~EntitiesFactory()
{
}


GameEntity* EntitiesFactory::Create(InventoryItem::ItemType type, const glm::vec3& position, GameScene* scene)
{
	GameEntity* entity = nullptr;

	switch (type)
	{
	case InventoryItem::ITEM_SHOT_GUN:
		break;
	case InventoryItem::ITEM_WATER_BATTERY:
	{
		Transformation* transformation = new Transformation(position, glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.0003f));

		entity = NGenius::GetInstance().CreateGameEntityFromModel("battery", transformation);
		entity->GetRenderer()->GetMaterial()->RemoveEffect<MaterialEffectDirectionalLightProperties>();

		Battery* battery = new Battery(InventoryItem::ITEM_WATER_BATTERY, 100, *entity);
		assert(battery != nullptr);
		
		entity = battery;

		ParticlesEmitter* steamParticlesEmitter = static_cast<ParticlesEmitter*>(NGenius::GetInstance().GetParticlesEmitter("steam")->Clone());
		if (steamParticlesEmitter != nullptr) 
		{
			steamParticlesEmitter->SetRotationSpeed(0.1f, .05f);
			steamParticlesEmitter->SetSpawnArea(glm::vec3(-0.0, 0.0f, -0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			battery->AddComponent(new AttachGameEntityComponent(steamParticlesEmitter));
			steamParticlesEmitter->SetGameScene(scene);
			Transformation* transformation = steamParticlesEmitter->GetTransformation();
			transformation->SetPosition(battery->GetTransformation()->GetPosition());
		}

		scene->AddEntity(battery);

		break;
	}
	case InventoryItem::ITEM_INVALID:
	default:
		Log(Log::LOG_ERROR) << "Error, unknown item type! \n";
		break;
	}

	return entity;
}