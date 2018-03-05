#pragma once
#include "IMaterial.h"

class IShaderProgram;


class BasicMaterial : public IMaterial
{
public:
	BasicMaterial(IShaderProgram* shader);
	virtual ~BasicMaterial();

	BasicMaterial* DoClone() const override { return nullptr; }
};

