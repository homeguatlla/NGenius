#pragma once
#include "ModelShader.h"

class EnergyWallShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;
	int mLocationDepthTexture;
	int mLocationContactPoint;
	int mLocationScreenSize;
	int mLocationTimer;

public:
	explicit EnergyWallShader();
	~EnergyWallShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void GetAllUniformLocations();

private:
	IShaderProgram* CreateShader() override;
};

