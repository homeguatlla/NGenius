#pragma once

class IShaderProgram;
class ITexture;

class IMaterial
{
	static unsigned int IDCounter;

	unsigned int mMaterialID;
	IShaderProgram* mShader;
	ITexture* mDiffuseTexture;
	ITexture* mNormalmapTexture;
	

protected:

public:
	IMaterial(IShaderProgram* shader, ITexture* diffuse, ITexture* normal);
	virtual ~IMaterial();

	unsigned int GetMaterialID() const;
	
	IShaderProgram* GetShader();

	virtual IMaterial* Clone() const;

protected:
	virtual IMaterial* DoClone() const = 0;
};

