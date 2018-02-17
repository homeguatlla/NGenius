#include "stdafx.h"
#include "DiffuseTexture.h"

DiffuseTexture::DiffuseTexture(ITexture* texture, const glm::vec3& color, float tile) :
mTexture(texture),
mColor(color),
mTile(tile)
{
}


DiffuseTexture::~DiffuseTexture()
{
}

ITexture* DiffuseTexture::GetDiffuseTexture() const
{
	return mTexture;
}
const glm::vec3 DiffuseTexture::GetColor() const
{
	return mColor;
}

float DiffuseTexture::GetTile() const
{
	return mTile;
}

DiffuseTexture* DiffuseTexture::DoClone() const
{
	return new DiffuseTexture(*this);
}