#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class BrightShader : public IShaderProgram, public IFactory
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	BrightShader();
	virtual ~BrightShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:
	IShaderProgram* CreateShader();

private:
	int mLocationTexture;
	int mLocationBright;
};

