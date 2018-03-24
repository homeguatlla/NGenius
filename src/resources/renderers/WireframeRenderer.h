#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;

class WireframeRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	WireframeRenderer(Model* model, IMaterial* material);
	~WireframeRenderer();

	void Draw();

	WireframeRenderer* DoClone() const override { return nullptr; }

private:
	bool IsInstancingAllowed() const override;
};

