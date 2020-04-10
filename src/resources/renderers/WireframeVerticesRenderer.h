#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;

class WireframeVerticesRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	WireframeVerticesRenderer(Model* model, IMaterial* material);
	~WireframeVerticesRenderer();

	void Draw();

	WireframeVerticesRenderer* DoClone() const override { return nullptr; }
	unsigned int GetNumberTrianglesRendered() const override;

private:
	bool IsInstancingAllowed() const override;
};

