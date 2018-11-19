#include "stdafx.h"
#include "Battery.h"
#include "../components/PickedUpComponent.h"
#include "../../resources/scene/GameScene.h"
#include "../../resources/entities/ParticlesEmitter.h"

Battery::Battery(Item::ItemType type, unsigned int power, GameEntity& entity, ParticlesEmitter* emitter) :
	mType(type),
	mPower(power),
	mSteamParticleEmitter(emitter),
	GameEntity(entity)
{
	AddComponent(new PickedUpComponent(type, power));
	if (emitter != nullptr)
	{
		Transformation* transformation = emitter->GetTransformation();
		transformation->SetPosition(entity.GetTransformation()->GetPosition());
	}
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

ParticlesEmitter* Battery::GetParticlesEmitter()
{
	return mSteamParticleEmitter;
}