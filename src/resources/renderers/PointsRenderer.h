#pragma once
#include "IRenderer.h"
#include "../Memory.h"
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
	static std::string GetClassName() { return std::string("PointsRenderer"); }
	static IRenderer* Create(Model* model, IMaterial* material)
	{
		return DBG_NEW PointsRenderer(model, material);
	}
	void Draw() override;

	unsigned int GetNumberTrianglesRendered() const override;
private:
	bool IsInstancingAllowed() const override;
};

