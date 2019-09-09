#pragma once
#include "IRenderer.h"

class IShaderProgram;
class Model;
class IMaterial;

class ParticleRenderer : public IRenderer
{

public:
	explicit ParticleRenderer(Model* model, IMaterial* material);
	~ParticleRenderer();

	ParticleRenderer* DoClone() const override { return nullptr; }
	bool IsInstancingAllowed() const override;
};

