#pragma once
#include "IRenderer.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class PointsRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	PointsRenderer(Model* model, IMaterial* material);
	~PointsRenderer();

	PointsRenderer* DoClone() const override { return nullptr; }
	//const std::string GetName() const override;
	void Draw() override;

	unsigned int GetNumberTrianglesRendered() const override;
private:
	bool IsInstancingAllowed() const override;
};

