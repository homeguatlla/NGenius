#include "stdafx.h"
#include "ModelShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectShadowProperties.h"
#include "../textures/ITexture.h"

const std::string ModelShader::VERTEX_FILE = "data/shaders/vertex/v_model.cg";
const std::string ModelShader::FRAGMENT_FILE = "data/shaders/fragment/f_model.cg";

const std::string ATTRIBUTE_CAMERA_POSITION("cameraPosition");
const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_VERTEX_NORMAL("normalModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_TILE("tile");

ModelShader::ModelShader() :
ModelShader(VERTEX_FILE, FRAGMENT_FILE)
{
	
}

ModelShader::ModelShader(const std::string& vertexFile, const std::string& fragmentFile) :
IShaderProgram(vertexFile, fragmentFile),
mLocationModelMatrix(-1),
mLocationCameraPosition(-1),
mLocationTextureCoords(-1),
mLocationNormal(-1),
mLocationTexture(-1),
mLocationTile(-1)
{
}


ModelShader::~ModelShader()
{
}

void ModelShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	IShaderProgram::LoadData(camera, transformation, material);

	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());
	LoadVector3(mLocationCameraPosition, camera->GetPosition());

	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
		LoadFloat(mLocationTile, effectDiffuse->GetTile());
	}
}

void ModelShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationNormal, ATTRIBUTE_VERTEX_NORMAL);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void ModelShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationNormal = GetAttributeLocation(ATTRIBUTE_VERTEX_NORMAL);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);

	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationCameraPosition = GetUniformLocation(ATTRIBUTE_CAMERA_POSITION);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
}

IShaderProgram * ModelShader::CreateShader()
{
	return new ModelShader();
}
