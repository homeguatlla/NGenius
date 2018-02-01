#pragma once
#include "IShaderProgram.h"
#include "../../resources/entities/Light.h"

class NormalMapShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationLightPosition;
	int mLocationLightColor;
	int mLocationCameraPosition;
	int mLocationTextureCoords;
	int mLocationNormal;
	int mLocationTexture;
	int mLocationFogDensity;
	int	mLocationFogGradient;
	int mLocationFogColor;
	int mLocationTiling; 
	int mLocationTangent;
	int mLocationNormalmapTexture;
	int mLocationShadowSpaceMatrix;
	int mLocationShadowMapTexture;
	int mLocationShadowMapTextureWidth;
	int mLocationShadowMapPFC;

public:
	NormalMapShader();
	~NormalMapShader();

	void BindAttributes();
	void GetAllUniformLocations();

	void LoadLight(const Light& light);
	void LoadViewMatrix(const glm::mat4& modelview);
	void LoadProjectionMatrix(const glm::mat4& projectionmatrix);
	void LoadCameraPosition(const glm::vec3& position);
	void LoadModelTexture(int unit);
	void LoadFogParameters(const glm::vec3& color, float density, float gradient);
	void LoadTiling(float tile);
	void LoadModelNormalmap(int unit);
	void LoadShadowMapSpaceMatrix(const glm::mat4& matrix);
	void LoadShadowMapTexture(int unit, int width);
	void LoadShadowMapPFC(int pfcCounter);
};

