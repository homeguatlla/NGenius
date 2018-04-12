#pragma once
#include "IShaderProgram.h"

class HorizontalBlurShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	HorizontalBlurShader();
	~HorizontalBlurShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:

	int mLocationTexture;
	int mLocationWidth;
};

