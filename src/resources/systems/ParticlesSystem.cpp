#include "stdafx.h"
#include "ParticlesSystem.h"
#include "../entities/ParticlesEmitter.h"



ParticlesSystem::ParticlesSystem()
{
}


ParticlesSystem::~ParticlesSystem()
{
	mParticlesEmitters.clear();
}

void ParticlesSystem::Update(float deltaTime)
{
	/*
	for (ParticlesEmitter* emitter : mParticlesEmitters)
	{
	}*/
}

void ParticlesSystem::AddParticleEmitter(ParticlesEmitter* emitter)
{
	bool found = std::find(mParticlesEmitters.begin(), mParticlesEmitters.end(), emitter) != mParticlesEmitters.end();
	if (!found)
	{
		emitter->SetMaxParticlesUpdate(512);
		mParticlesEmitters.push_back(emitter);
	}
}