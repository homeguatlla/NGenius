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
	const std::string GetName() const override;
	bool IsInstancingAllowed() const override;

private:
	void Create();
	void Draw() override;
};

