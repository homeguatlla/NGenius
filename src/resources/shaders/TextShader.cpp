#include "stdafx.h"
#include "TextShader.h"
#include "IShaderProgram.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../textures/ITexture.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectText.h"

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

void TextShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
}

void TextShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());
	LoadMatrix4(mLocationModelMatrix, const_cast<Transformation*>(transformation)->GetModelMatrix());

	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
	}

	MaterialEffectText* effectText = material->GetEffect<MaterialEffectText>();
	if (effectText != nullptr)
	{
		LoadVector4(mLocationColor, effectText->GetColor());
		LoadVector4(mLocationOutlineColor, effectText->GetOutlineColor());
		LoadFloat(mLocationWidth, effectText->GetWidth());
		LoadFloat(mLocationEdge, effectText->GetEdge());
		LoadFloat(mLocationBorderWidth, effectText->GetBorderWidth());
		LoadFloat(mLocationBorderEdge, effectText->GetBorderEdge());
		LoadVector2(mLocationShadowOffset, effectText->GetShadowOffset());
	}
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

IShaderProgram* TextShader::CreateShader()
{
	return new TextShader();
}