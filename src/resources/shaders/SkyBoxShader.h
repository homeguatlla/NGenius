#pragma once
#include "IShaderProgram.h"

class SkyBoxShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationCubemapTexture;
	int mLocationFogColor;

public:
	SkyBoxShader();
	~SkyBoxShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void GetAllUniformLocations() override;
	void BindAttributes() override;
	void LoadModelMatrix(const glm::mat4& modelMatrix);
	void LoadViewMatrix(const glm::mat4& viewMatrix);
	void LoadProjectionMatrix(const glm::mat4& projectionMatrix);
	void LoadCubemapTexture(int unit);
	void LoadFogColor(const glm::vec3& fogColor);
};

