#pragma once
#include "IShaderProgram.h"


class WaterShader :	public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationReflectionTexture;
	int mLocationRefractionTexture;
	int mLocationDistorsionTexture;
	int mLocationNormalTexture;
	int mLocationDepthTexture;
	int mLocationTextureCoords;
	int mLocationWaterSpeed;
	int mLocationWaterColor;
	int mLocationCameraPosition;
	int mLocationLightPosition;
	int mLocationLightColor;
	int mLocationFogDensity;
	int	mLocationFogGradient;
	int mLocationFogColor;

public:
	WaterShader();
	~WaterShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadReflectionTexture(int unit);
	void LoadRefractionTexture(int unit);
	void LoadDistorsionTexture(int unit);
	void LoadNormalTexture(int unit);
	void LoadDepthTexture(int unit);
	void LoadWaterSpeed(float speed);
	void LoadWaterColor(const glm::vec4& color);
};

