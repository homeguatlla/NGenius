#pragma once
#include "VerticesRenderer.h"
#include<string>

class Model;
class IMaterial;
class Texture;
class ICamera;

class EnergyWallRenderer : public VerticesRenderer
{
	unsigned int mMatrixVBO;

public:
	EnergyWallRenderer(Model* model, IMaterial* material);
	~EnergyWallRenderer();

	EnergyWallRenderer* DoClone() const override { return nullptr; }

private:
	bool IsInstancingAllowed() const override;
};

