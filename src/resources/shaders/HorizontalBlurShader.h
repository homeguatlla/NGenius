#pragma once
#include "IShaderProgram.h"

class HorizontalBlurShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	HorizontalBlurShader();
	virtual ~HorizontalBlurShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("HorizontalBlurShader"); }
	static IShaderProgram* Create() { return new HorizontalBlurShader(); }

private:

	int mLocationTexture;
	int mLocationWidth;
};

