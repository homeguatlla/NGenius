#pragma once
#include "ModelShader.h"

class NormalMapShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationTangent;
	int mLocationNormalmapTexture;

public:
	NormalMapShader();
	~NormalMapShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes();
	void GetAllUniformLocations();

private:
	IShaderProgram* CreateShader() override;
};

