#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class GrassShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;
	static const std::string GEOMETRY_FILE;

	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	
	int mLocationTexture;
	int mLocationTile;
	
	int mLocationFogDensity;
	int	mLocationFogGradient;
	int mLocationFogColor;

	int mLocationSize;

	int mLocationShadowSpaceMatrix;
	int mLocationShadowMapTexture;
	int mLocationShadowMapTextureWidth;
	int mLocationShadowMapPFC;

	int mLocationTimer;
	int mLocationWindDirections;

	int mLocationDepthTexture;
	int mLocationScreenSize;

	int mLocationWindModificatorsArray;

public:
	GrassShader();
	virtual ~GrassShader() = default;

	virtual void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes();
	void GetAllUniformLocations();
	int GetGeometryShaderNumTriangles() const override;

	static std::string GetClassName() { return std::string("GrassShader"); }
	static IShaderProgram* Create() { return DBG_NEW GrassShader(); }
};

