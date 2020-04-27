#pragma once
#include "src/utils/serializer/ISerializable.h"
#include <vector>
#include <memory>
class ParticlesEmitter;

class ParticlesSystem : public core::utils::ISerializable
{
	std::vector<std::shared_ptr<ParticlesEmitter>> mParticlesEmitters;
	const unsigned long MAX_PARTICLES = 1024;
	
public:
	ParticlesSystem();
	~ParticlesSystem();

	void Update(float deltaTime);
	void AddParticleEmitter(std::shared_ptr<ParticlesEmitter> emitter);

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;
};

