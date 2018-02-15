#pragma once
#include "IShaderProgram.h"

class ShadowShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	ShadowShader();
	~ShadowShader();

	void LoadData(const ICamera* camera, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadMVPMatrix(const glm::mat4& matrix);

private:

	int mLocationMVP;
};

