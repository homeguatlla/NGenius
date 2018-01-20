#pragma once
#include "IShaderProgram.h"


class QuadShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationTexture;
	int mLocationTextureCoords;
public:

	QuadShader();
	~QuadShader();

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadViewMatrix(const glm::mat4& modelview);
	void LoadProjectionMatrix(const glm::mat4& projectionmatrix);
	void LoadQuadTexture(int unit);
};

