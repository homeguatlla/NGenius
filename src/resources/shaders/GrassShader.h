#pragma once
#include "IShaderProgram.h"

class GrassShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;
	static const std::string GEOMETRY_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	
	int mLocationTexture;
	int mLocationTile;
	
	int mLocationFogDensity;
	int	mLocationFogGradient;
	int mLocationFogColor;

	int mLocationSize;

public:
	GrassShader();
	~GrassShader();

	virtual void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes();
	void GetAllUniformLocations();


};

