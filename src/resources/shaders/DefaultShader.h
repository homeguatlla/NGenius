#pragma once
#include "IShaderProgram.h"


class DefaultShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	DefaultShader();
	~DefaultShader();

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadTransformationMatrix(const glm::mat4& matrix);

private:
	
	int mLocationMVP;
};

