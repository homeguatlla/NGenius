#pragma once
#include "IShaderProgram.h"

class DefaultShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	DefaultShader();
	~DefaultShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:
	
	int mLocationM;
	int mLocationV;
	int mLocationP;
};

