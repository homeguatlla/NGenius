#pragma once
#include "../../utils/serializer/ISerializable.h"
#include <vector>

class ParticlesEmitter;

class ParticlesSystem : public core::utils::ISerializable
{
	std::vector<ParticlesEmitter*> mParticlesEmitters;
	const unsigned long MAX_PARTICLES = 1024;
	
public:
	ParticlesSystem();
	~ParticlesSystem();

	void Update(float deltaTime);
	void AddParticleEmitter(ParticlesEmitter* emitter);

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;
};

