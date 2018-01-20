#pragma once
#include "IShaderProgram.h"
#include "../../resources/entities/Light.h"

class TerrainShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	TerrainShader();
	~TerrainShader();

	void BindAttributes();
	void GetAllUniformLocations();

	//TODO light debería ser const
	void LoadLight(const Light& light);
	void LoadModelMatrix(const glm::mat4& modelmatrix);
	void LoadCameraPosition(const glm::vec3& position);
	void LoadScale(float scale);
	void LoadArrayTexture(int index);
	void LoadHeightMapTexture(int unit);
	void LoadBlendMapTexture(int unit);
	void LoadTile(float tile);
	void LoadFogParameters(const glm::vec3& color, float density, float gradient);
	void LoadClippingPlane(const glm::vec4 plane);

private:
	int mLocationMVPMatrix;
	int mLocationModelMatrix;
	int mLocationLightPosition;
	int mLocationLightColor;
	int mLocationCameraPosition;
	int mLocationScale;
	int mLocationTextureCoords;
	int mLocationHeightMapTexture;
	int mLocationBlendMapTexture;
	int mLocationArrayTexture;
	int mLocationTile;
	int mLocationFogDensity;
	int	mLocationFogGradient;
	int mLocationFogColor;
	int mLocationClippingPlane;
};

