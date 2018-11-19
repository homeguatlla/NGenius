#pragma once

#include <assert.h>
#include "../BaseLibrary.h"


class ParticlesEmitter;
class NGenius;

class ParticlesEmitterLibrary : public BaseLibrary<ParticlesEmitter*>
{
public:
	
	ParticlesEmitterLibrary();
	virtual ~ParticlesEmitterLibrary();

	void Load(NGenius* engine);

private:
	void CreateSteamParticlesEmitter(const std::string& name, NGenius* engine);
};

