#pragma once
#include "IRenderer.h"

class Model;
class IMaterial;

class SkyBoxRenderer :	public IRenderer
{
	float mSize;

public:
	explicit SkyBoxRenderer(Model* model, IMaterial* material);
	~SkyBoxRenderer();

	SkyBoxRenderer* DoClone() const override { return nullptr; }
	bool IsInstancingAllowed() const override;

	static std::string GetClassName() { return std::string("SkyBoxRenderer"); }
	static IRenderer* Create(Model* model, IMaterial* material)
	{
		return new SkyBoxRenderer(model, material);
	}

private:
	void Draw() override;
};

