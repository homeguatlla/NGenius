#include "stdafx.h"
#include "DiffuseTexture.h"

DiffuseTexture::DiffuseTexture(ITexture* texture) :
mTexture(texture)
{
}


DiffuseTexture::~DiffuseTexture()
{
}

ITexture* DiffuseTexture::GetDiffuseTexture() const
{
	return mTexture;
}

DiffuseTexture* DiffuseTexture::DoClone() const
{
	return new DiffuseTexture(*this);
}