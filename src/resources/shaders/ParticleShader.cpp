#include "stdafx.h"
#include "ParticleShader.h"

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

void ParticleShader::LoadData(const ICamera* camera, IMaterial* material)
{

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

void ParticleShader::LoadViewMatrix(const glm::mat4& viewmatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewmatrix);
}

void ParticleShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void ParticleShader::LoadQuadTexture(int unit)
{
	LoadTexture(mLocationTexture, unit);
}

void ParticleShader::LoadDepthTexture(int unit)
{
	LoadTexture(mLocationDepthTexture, unit);
}

void ParticleShader::LoadTile(float tile)
{
	LoadFloat(mLocationTile, tile);
}

void ParticleShader::LoadTime(float time)
{
	LoadFloat(mLocationTime, time);
}

void ParticleShader::LoadScreenSize(const glm::vec2& size)
{
	LoadVector2(mLocationScreenSize, size);
}