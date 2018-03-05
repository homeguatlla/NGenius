#pragma once

#include <assert.h>
#include "../BaseLibrary.h"


class IMaterial;
class ITexture;
class IShaderProgram;

class MaterialsLibrary : public BaseLibrary<IMaterial*>
{

public:
	MaterialsLibrary();
	virtual ~MaterialsLibrary();

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void Load();
};

