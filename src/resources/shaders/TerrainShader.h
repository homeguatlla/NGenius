#pragma once
#include "IShaderProgram.h"

class TerrainShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	TerrainShader();
	~TerrainShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes();
	void GetAllUniformLocations();

	void LoadScale(float scale);
	void LoadArrayTexture(int index);
	void LoadHeightMapTexture(int unit);
	void LoadClippingPlane(const glm::vec4& plane);
	
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
	int mLocationShadowSpaceMatrix;
	int mLocationShadowMapTexture;
	int mLocationShadowMapTextureWidth;
	int mLocationShadowMapPFC;
};

