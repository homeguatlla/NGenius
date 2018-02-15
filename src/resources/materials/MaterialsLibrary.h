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

	void CreateMaterial(const std::string& name, IShaderProgram* shader, ITexture* diffuseTexture, ITexture* normalTexture);

	void Load();
};

