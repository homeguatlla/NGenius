#pragma once
#include "IndicesRenderer.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class EnergyWallRenderer : public IndicesRenderer
{
	unsigned int mMatrixVBO;

public:
	EnergyWallRenderer(Model* model, IMaterial* material);
	~EnergyWallRenderer();

	EnergyWallRenderer* DoClone() const override { return nullptr; }
	void Draw() override;

private:
	bool IsInstancingAllowed() const override;
};

