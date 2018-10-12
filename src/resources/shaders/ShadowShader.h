#pragma once
#include "IShaderProgram.h"

class ShadowShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	ShadowShader();
	~ShadowShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:

	int mLocationM;
	int mLocationV;
	int mLocationP;
	int mLocationTextureCoords;
	int mLocationTexture;
	int mLocationTile;
	int mLocationJointTransformMatrixs;
	int mLocationJointTransformSize;
};

