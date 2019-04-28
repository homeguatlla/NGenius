#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class DefaultShader : public IShaderProgram, public IFactory
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	DefaultShader();
	virtual ~DefaultShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:
	IShaderProgram* CreateShader() override;

private:
	
	int mLocationM;
	int mLocationV;
	int mLocationP;
	int mLocationColor;
};

