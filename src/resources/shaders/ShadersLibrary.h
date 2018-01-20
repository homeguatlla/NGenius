#pragma once
#include <map>
#include "../BaseLibrary.h"

class IShaderProgram;

class ShadersLibrary : public BaseLibrary<IShaderProgram*>
{
public:
	ShadersLibrary();
	~ShadersLibrary();

	void Load();
};

