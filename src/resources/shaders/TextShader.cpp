#include "stdafx.h"
#include "TextShader.h"
#include "IShaderProgram.h"

const std::string TextShader::VERTEX_FILE = "data/shaders/vertex/v_text.cg";
const std::string TextShader::FRAGMENT_FILE = "data/shaders/fragment/f_text.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_COLOR("fontColor");
const std::string ATTRIBUTE_OUTLINE_COLOR("outlineColor");
const std::string ATTRIBUTE_WIDTH("width");
const std::string ATTRIBUTE_EDGE("edge");
const std::string ATTRIBUTE_BORDER_WIDTH("borderWidth");
const std::string ATTRIBUTE_BORDER_EDGE("borderEdge");
const std::string ATTRIBUTE_SHADOW_OFFSET("shadowOffset");

TextShader::TextShader() : IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationModelMatrix(-1),
mLocationViewMatrix(-1),
mLocationProjectionMatrix(-1),
mLocationTexture(-1),
mLocationTextureCoords(-1),
mLocationColor(-1),
mLocationOutlineColor(-1),
mLocationWidth(-1),
mLocationEdge(-1),
mLocationBorderWidth(-1),
mLocationBorderEdge(-1),
mLocationShadowOffset(-1)
{
}


TextShader::~TextShader()
{
}

void TextShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
}

void TextShader::LoadData(const ICamera* camera, IMaterial* material)
{

}

void TextShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationModelMatrix = GetUniformLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationColor = GetUniformLocation(ATTRIBUTE_COLOR);
	mLocationOutlineColor = GetUniformLocation(ATTRIBUTE_OUTLINE_COLOR);
	mLocationWidth = GetUniformLocation(ATTRIBUTE_WIDTH);
	mLocationEdge = GetUniformLocation(ATTRIBUTE_EDGE);
	mLocationBorderWidth = GetUniformLocation(ATTRIBUTE_BORDER_WIDTH);
	mLocationBorderEdge = GetUniformLocation(ATTRIBUTE_BORDER_EDGE);
	mLocationShadowOffset = GetUniformLocation(ATTRIBUTE_SHADOW_OFFSET);
}

void TextShader::LoadViewMatrix(const glm::mat4& viewmatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewmatrix);
}

void TextShader::LoadModelMatrix(const glm::mat4& modelmatrix)
{
	LoadMatrix4(mLocationModelMatrix, modelmatrix);
}

void TextShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void TextShader::LoadFontTypeTexture(int unit)
{
	LoadTexture(mLocationTexture, unit);
}

void TextShader::LoadColor(const glm::vec4& color)
{
	LoadVector4(mLocationColor, color);
}

void TextShader::LoadOutlineColor(const glm::vec4& color)
{
	LoadVector4(mLocationOutlineColor, color);
}

void TextShader::LoadBorderParameters(float width, float edge, float borderWidth, float borderEdge)
{
	LoadFloat(mLocationWidth, width);
	LoadFloat(mLocationEdge, edge);
	LoadFloat(mLocationBorderWidth, borderWidth);
	LoadFloat(mLocationBorderEdge, borderEdge);
}

void TextShader::LoadShadow(const glm::vec2& offset)
{
	LoadVector2(mLocationShadowOffset, offset);
}
