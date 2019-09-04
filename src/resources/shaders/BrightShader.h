#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class BrightShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	BrightShader();
	virtual ~BrightShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("BrightShader"); }
	static IShaderProgram* Create() { return DBG_NEW BrightShader(); }

private:
	int mLocationTexture;
	int mLocationBright;
};

