#pragma once
#include "IRenderer_.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class VertexsRenderer : public IRenderer_
{
	unsigned int mMatrixVBO;

public:
	VertexsRenderer(Model* model, IMaterial* material);
	~VertexsRenderer();

	VertexsRenderer* DoClone() const override { return nullptr; }
	//const std::string GetName() const override;
	void Draw() override;

private:
	bool IsInstancingAllowed() const override;
};

