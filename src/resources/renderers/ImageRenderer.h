#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;

class ImageRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	ImageRenderer(Model* model, IMaterial* material);
	~ImageRenderer();
	
	void Render();

	ImageRenderer* DoClone() const override { return nullptr; }
	
private:
	bool IsInstancingAllowed() const override;
};

