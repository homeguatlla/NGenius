#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class BloomShader : public IShaderProgram, public IFactory
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	BloomShader();
	virtual ~BloomShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:
	IShaderProgram* CreateShader() override;

private:
	int mLocationTexture;
	int mLocationBrightTexture;
};

