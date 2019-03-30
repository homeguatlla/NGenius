#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class TerrainShader : public IShaderProgram, public IFactory
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	TerrainShader();
	~TerrainShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes();
	void GetAllUniformLocations();

private:
	IShaderProgram* CreateShader() override;

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
};

