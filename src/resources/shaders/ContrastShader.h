#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class ContrastShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	ContrastShader();
	virtual ~ContrastShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("ContrastShader"); }
	static IShaderProgram* Create() { return DBG_NEW ContrastShader(); }

private:
	int mLocationTexture;
	int mLocationContrast;
};

