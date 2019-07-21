#pragma once
#include <vector>

class ParticlesEmitter;

class ParticlesSystem
{
	std::vector<ParticlesEmitter*> mParticlesEmitters;
	const unsigned long MAX_PARTICLES = 1024;
	
public:
	ParticlesSystem();
	~ParticlesSystem();

	void Update(float deltaTime);
	void AddParticleEmitter(ParticlesEmitter* emitter);
};

