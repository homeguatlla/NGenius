#include "stdafx.h"
#include "ParticleShader.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectParticle.h"
#include "../textures/ITexture.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"

const std::string ParticleShader::VERTEX_FILE = "data/shaders/vertex/v_particle.cg";
const std::string ParticleShader::FRAGMENT_FILE = "data/shaders/fragment/f_particle.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_DEPTH_TEXTURE("depthTexture");
const std::string ATTRIBUTE_TILE("tile");
const std::string ATTRIBUTE_TIME("time");
const std::string ATTRIBTUTE_SCREEN_SIZE("screenSize");

ParticleShader::ParticleShader() :
ParticleShader(VERTEX_FILE, FRAGMENT_FILE)
{
}

ParticleShader::ParticleShader(const std::string& vertexFile, const std::string& fragmentFile) :
IShaderProgram(vertexFile, fragmentFile),
mLocationModelMatrix(-1),
mLocationViewMatrix(-1),
mLocationProjectionMatrix(-1),
mLocationTexture(-1),
mLocationDepthTexture(-1),
mLocationTextureCoords(-1),
mLocationTile(-1),
mLocationTime(-1),
mLocationScreenSize(-1)
{

}

ParticleShader::~ParticleShader()
{
}

void ParticleShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());

	MaterialEffectParticle* effect = material->GetEffect<MaterialEffectParticle>();
	if (effect != nullptr)
	{
		LoadTexture(mLocationTexture, effect->GetTexture()->GetUnit());
		LoadTexture(mLocationDepthTexture, effect->GetDepthTexture()->GetUnit());
		LoadFloat(mLocationTile, effect->GetTile());
		LoadVector2(mLocationScreenSize, effect->GetScreenSize());
	}
}

void ParticleShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void ParticleShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
	mLocationTime = GetUniformLocation(ATTRIBUTE_TIME);
	mLocationScreenSize = GetUniformLocation(ATTRIBTUTE_SCREEN_SIZE);
}