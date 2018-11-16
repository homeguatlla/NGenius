#include "stdafx.h"
#include "Battery.h"
#include "../components/PickedUpComponent.h"
#include "../../resources/scene/GameScene.h"
#include "../../resources/entities/ParticlesEmitter.h"

Battery::Battery(Item::ItemType type, unsigned int power, GameEntity& entity) :
	mType(type), 
	mPower(power), 
	GameEntity(entity)
{
	AddComponent(new PickedUpComponent(type, power));
	//create steam
}

Battery::~Battery()
{
}

Item::ItemType Battery::GetType() const
{
	return mType;
}

unsigned int Battery::GetPower() const
{
	return mPower;
}

void Battery::AddIntoScene(GameScene* scene)
{
	scene->AddEntity(this);
	scene->AddEntity(mSteamParticleEmitter);
}
