#include "stdafx.h"
#include "HeightMapTexture.h"

HeightMapTexture::HeightMapTexture(ITexture* texture, float tile) :
mTexture(texture),
mTile(tile)
{
}


HeightMapTexture::~HeightMapTexture()
{
}

ITexture* HeightMapTexture::GetHeightMapTexture() const
{
	return mTexture;
}

float HeightMapTexture::GetTile() const
{
	return mTile;
}

HeightMapTexture* HeightMapTexture::DoClone() const
{
	return new HeightMapTexture(*this);
}