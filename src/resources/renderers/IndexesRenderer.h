#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;

class IndexesRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	IndexesRenderer(Model* model, IMaterial* material);
	~IndexesRenderer();

	IndexesRenderer* DoClone() const override { return nullptr; }
	//const std::string GetName() const override;

private:
	bool IsInstancingAllowed() const override;
};

