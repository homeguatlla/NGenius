#pragma once
#include "IRenderer_.h"

class IShaderProgram;
class Model;
class IMaterial;

class ParticleRenderer : public IRenderer_
{

public:
	explicit ParticleRenderer(Model* model, IMaterial* material);
	~ParticleRenderer();

	ParticleRenderer* DoClone() const override { return nullptr; }
	bool IsInstancingAllowed() const override;

private:
	void Draw() override;
};

