#pragma once
#include "IShaderProgram.h"


class GUIShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationTexture;
	int mLocationTextureCoords;
public:

	GUIShader();
	~GUIShader();

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadViewMatrix(const glm::mat4& modelview);
	void LoadProjectionMatrix(const glm::mat4& projectionmatrix);
	void LoadGUITexture(int unit);
};

