#include "stdafx.h"
#include "DiffuseTexture.h"

DiffuseTexture::DiffuseTexture(ITexture* texture, int tile) :
mTexture(texture),
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

int DiffuseTexture::GetTile() const
{
	return mTile;
}

DiffuseTexture* DiffuseTexture::DoClone() const
{
	return new DiffuseTexture(*this);
}