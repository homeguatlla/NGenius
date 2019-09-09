#pragma once
#include "IRenderer.h"
#include "../../Memory.h"

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
		return DBG_NEW SkyBoxRenderer(model, material);
	}
};

