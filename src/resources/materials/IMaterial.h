#pragma once

class IShaderProgram;
class ITexture;

class IMaterial
{
	IShaderProgram* mShader;
	ITexture* mDiffuseTexture;
	ITexture* mNormalmapTexture;
	

protected:

public:
	IMaterial(IShaderProgram* shader, ITexture* diffuse, ITexture* normal);
	virtual ~IMaterial();
};

