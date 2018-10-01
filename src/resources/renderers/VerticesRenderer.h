#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class VerticesRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	VerticesRenderer(Model* model, IMaterial* material);
	~VerticesRenderer();

	VerticesRenderer* DoClone() const override { return nullptr; }
	//const std::string GetName() const override;
	void Draw() override;

	unsigned int GetNumberTrianglesRendered() const override;
private:
	bool IsInstancingAllowed() const override;
};

