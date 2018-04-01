#pragma once
#include "IShaderProgram.h"

class OverdrawShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	OverdrawShader();
	~OverdrawShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:

	int mLocationM;
	int mLocationV;
	int mLocationP;
};
