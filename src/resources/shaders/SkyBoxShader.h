#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class SkyBoxShader : public IShaderProgram, public IFactory
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
	virtual ~SkyBoxShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:
	IShaderProgram* CreateShader() override;
};

