#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class VertexsRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	VertexsRenderer(Model* model, IMaterial* material);
	~VertexsRenderer();

	VertexsRenderer* DoClone() const override { return nullptr; }
	//const std::string GetName() const override;
	void Draw() override;

	unsigned int GetNumberTrianglesRendered() const override;
private:
	bool IsInstancingAllowed() const override;
};

