#pragma once
#include "IShaderProgram.h"

class OverdrawShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	OverdrawShader();
	virtual ~OverdrawShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("OverdrawShader"); }
	static IShaderProgram* Create() { return new OverdrawShader(); }

private:

	int mLocationM;
	int mLocationV;
	int mLocationP;
};

