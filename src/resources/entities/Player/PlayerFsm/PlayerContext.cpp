#include "stdafx.h"
#include "PlayerContext.h"
#include "src/resources/entities/Player/Player.h"
#include "src/resources/components/CharacterComponent.h"
#include "src/resources/components/PhysicsComponent.h"

PlayerContext::PlayerContext(std::shared_ptr<Player> player) :
	mPlayer { player},
	mCharacterComponent { std::shared_ptr<CharacterComponent>(player->GetComponent<CharacterComponent>())},
	mPhysicsComponent{ std::shared_ptr<PhysicsComponent>(player->GetComponent<PhysicsComponent>()) }
{
}