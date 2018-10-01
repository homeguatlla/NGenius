#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;

class IndicesRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	IndicesRenderer(Model* model, IMaterial* material);
	~IndicesRenderer();

	IndicesRenderer* DoClone() const override { return nullptr; }
	//const std::string GetName() const override;

private:
	bool IsInstancingAllowed() const override;
};

