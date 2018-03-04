#pragma once
#include "IRenderer.h"

class Model;
class IMaterial;

class SkyBoxRenderer :	public IRenderer
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

