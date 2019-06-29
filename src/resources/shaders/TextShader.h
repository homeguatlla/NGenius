#pragma once
#include "IShaderProgram.h"

class TextShader : public IShaderProgram
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
	virtual ~TextShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("TextShader"); }
	static IShaderProgram* Create() { return new TextShader(); }
};

