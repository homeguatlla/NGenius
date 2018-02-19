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
	~TextShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadFontTypeTexture(int unit);
	void LoadColor(const glm::vec4& color);
	void LoadOutlineColor(const glm::vec4& color);
	void LoadBorderParameters(float width, float edge, float borderWidth, float borderEdge);
	void LoadShadow(const glm::vec2& offset);
};

