#pragma once
#include "IShaderProgram.h"
#include "../../resources/entities/Light.h"

class ModelShader :	public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	ModelShader();
	ModelShader(const std::string& vertexFile, const std::string& fragmentFile);
	~ModelShader();

	void BindAttributes();
	void GetAllUniformLocations();

	void LoadLight(const Light& light);
	void LoadViewMatrix(const glm::mat4& modelview);
	void LoadProjectionMatrix(const glm::mat4& projectionmatrix);
	void LoadCameraPosition(const glm::vec3& position);
	void LoadModelTexture(int unit);
	void LoadFogParameters(const glm::vec3& color, float density, float gradient);
	void LoadTile(float tile);
	void LoadTime(float time);
	void LoadShadowMapSpaceMatrix(const glm::mat4& matrix);
	void LoadShadowMapTexture(int unit, int width);
	void LoadShadowMapPFC(int pfcCounter);

private:
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
	int mLocationTile;
	int mLocationTime;
	int mLocationShadowSpaceMatrix;
	int mLocationShadowMapTexture;
	int mLocationShadowMapTextureWidth;
	int mLocationShadowMapPFC;
};

