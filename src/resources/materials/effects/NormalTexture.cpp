#include "stdafx.h"
#include "NormalTexture.h"

NormalTexture::NormalTexture(ITexture* texture, int tile) :
mTexture(texture),
mTile(tile)
{
}


NormalTexture::~NormalTexture()
{
}

ITexture* NormalTexture::GetNormalTexture() const
{
	return mTexture;
}

int NormalTexture::GetTile() const
{
	return mTile;
}

NormalTexture* NormalTexture::DoClone() const
{
	return new NormalTexture(*this);
}