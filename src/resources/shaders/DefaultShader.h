#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class DefaultShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	DefaultShader();
	virtual ~DefaultShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("DefaultShader"); }
	static IShaderProgram* Create() { return DBG_NEW DefaultShader(); }

private:
	
	int mLocationM;
	int mLocationV;
	int mLocationP;
	int mLocationColor;
};

