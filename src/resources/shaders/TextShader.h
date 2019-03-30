#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class TextShader : public IShaderProgram, public IFactory
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationTexture;
	int mLocationTextureCoords;
	int mLocationColor;
	int mLocationOutlineColor;
	int mLocationWidth;
	int mLocationEdge;
	int mLocationBorderWidth;
	int mLocationBorderEdge;
	int mLocationShadowOffset;

public:
	TextShader();
	~TextShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

private:
	IShaderProgram* CreateShader() override;
};

