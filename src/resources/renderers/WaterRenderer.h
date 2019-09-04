#pragma once
#include "IndicesRenderer.h"
#include<string>

class WaterRenderer : public IndicesRenderer
{
public:
	WaterRenderer(Model* model, IMaterial* material);
	~WaterRenderer();
	void Draw() override;

	WaterRenderer* DoClone() const override { return nullptr; }
	static std::string GetClassName() { return std::string("WaterRenderer"); }
	static IRenderer* Create(Model* model, IMaterial* material)
	{
		return DBG_NEW WaterRenderer(model, material);
	}
};

