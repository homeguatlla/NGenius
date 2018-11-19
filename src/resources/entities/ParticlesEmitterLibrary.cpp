#include "stdafx.h"
#include "ParticlesEmitterLibrary.h"
#include "Particle.h"
#include "ParticlesEmitter.h"

#include "../../NGenius.h"
#include "../../resources/textures/ITexture.h"
#include "../../resources/textures/Texture.h"

#include <GL/glew.h>
#include <algorithm>


ParticlesEmitterLibrary::ParticlesEmitterLibrary() 
{
}

ParticlesEmitterLibrary::~ParticlesEmitterLibrary()
{

}

void ParticlesEmitterLibrary::CreateSteamParticlesEmitter(const std::string& name, NGenius* engine)
{
	Texture* texture = static_cast<Texture*>(engine->GetTexture("smoke"));
	Particle* particle = engine->CreateParticle(texture);
	if (particle != nullptr)
	{
		particle->SetLiveTime(3.0f);

		ParticlesEmitter* particlesEmitter = new ParticlesEmitter(particle,
			new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
			nullptr,
			10);
		particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		particlesEmitter->SetScaleValues(0.3f, 0.01f);
		particlesEmitter->SetVelocity(glm::vec3(0.0f), glm::vec3(0.0f, 3.0f, 0.0f));
		particlesEmitter->SetSpawnArea(glm::vec3(-0.06f, 0.0f, -0.06f), glm::vec3(0.06f, 0.0f, 0.06f));
		
		AddElement(name, particlesEmitter);
	}
}

void ParticlesEmitterLibrary::Load(NGenius* engine)
{
	CreateSteamParticlesEmitter("steam", engine);
}