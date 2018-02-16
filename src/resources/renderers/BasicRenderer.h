#pragma once
#include "IRenderer_.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class BasicRenderer : public IRenderer_
{
	unsigned int mMatrixVBO;

public:
	BasicRenderer(Model* model, IMaterial* material);
	~BasicRenderer();

	BasicRenderer* DoClone() const override { return nullptr; }
	const std::string GetName() const override;
	
private:
	bool IsInstancingAllowed() const override;
};

