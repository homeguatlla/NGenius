#pragma once

#include <assert.h>
#include "../BaseLibrary.h"


class IMaterial;
class ITexture;
class IShaderProgram;
class ShadersLibrary;


class MaterialsLibrary : public BaseLibrary<IMaterial*>
{
	ShadersLibrary* mShadersLibrary;
public:
	static const char* OVERDRAW_MATERIAL_NAME;
	static const char* WIREFRAME_MATERIAL_NAME;

	MaterialsLibrary(ShadersLibrary* shadersLibrary);
	virtual ~MaterialsLibrary();

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void Load();
};

