#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class VerticalBlurShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	VerticalBlurShader();
	virtual ~VerticalBlurShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("VerticalBlurShader"); }
	static IShaderProgram* Create() { return DBG_NEW VerticalBlurShader(); }

private:

	int mLocationTexture;
	int mLocationHeight;
};

