#pragma once
#include "IShaderProgram.h"


class Light;

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

	void LoadData(const ICamera* camera, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadViewMatrix(const glm::mat4& modelview);
	void LoadProjectionMatrix(const glm::mat4& projectionmatrix);
	void LoadReflectionTexture(int unit);
	void LoadRefractionTexture(int unit);
	void LoadDistorsionTexture(int unit);
	void LoadNormalTexture(int unit);
	void LoadDepthTexture(int unit);
	void LoadWaterSpeed(float speed);
	void LoadWaterColor(const glm::vec4& color);
	void LoadCameraPosition(const glm::vec3& position);
	void LoadLight(const Light* llight);
	void LoadFogParameters(const glm::vec3& color, float density, float gradient);
};

