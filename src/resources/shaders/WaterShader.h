#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

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


public:
	WaterShader();
	virtual ~WaterShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("WaterShader"); }
	static IShaderProgram* Create() { return DBG_NEW WaterShader(); }
};

