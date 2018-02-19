#pragma once
#include "IRenderer_.h"

class Model;
class IMaterial;

class SkyBoxRenderer :	public IRenderer_
{
	float mSize;

public:
	SkyBoxRenderer(Model* model, IMaterial* material);
	~SkyBoxRenderer();

	SkyBoxRenderer* DoClone() const override { return nullptr; }
	bool IsInstancingAllowed() const override;

private:
	void Draw() override;
};

