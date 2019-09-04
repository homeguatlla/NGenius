#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

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

	static std::string GetClassName() { return std::string("TerrainShader"); }
	static IShaderProgram* Create() { return DBG_NEW TerrainShader(); }

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
	int mLocationNormal;
	int mLocationTangent;
};

