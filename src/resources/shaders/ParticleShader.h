#pragma once
#include "IShaderProgram.h"


class ParticleShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationTexture;
	int mLocationDepthTexture;
	int mLocationTextureCoords;
	int mLocationTile;
	int mLocationTime;
	int mLocationScreenSize;

public:

	ParticleShader();
	ParticleShader(const std::string& vertexFile, const std::string& fragmentFile);
	~ParticleShader();

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadViewMatrix(const glm::mat4& modelview);
	void LoadProjectionMatrix(const glm::mat4& projectionmatrix);
	void LoadQuadTexture(int unit);
	void LoadDepthTexture(int unit);
	void LoadTile(float tile);
	void LoadTime(float time);
	void LoadScreenSize(const glm::vec2& size);
};

