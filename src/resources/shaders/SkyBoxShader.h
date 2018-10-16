#pragma once
#include "IShaderProgram.h"

class SkyBoxShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationCubemap1Texture;
	int mLocationCubemap2Texture;
	int mLocationBlendFactor;

public:
	SkyBoxShader();
	~SkyBoxShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void GetAllUniformLocations() override;
	void BindAttributes() override;
};

