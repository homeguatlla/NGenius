#pragma once

#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <glm/glm.hpp>
#include "../BaseLibrary.h"


class IMaterial;

class MaterialsLibrary : public BaseLibrary<IMaterial*>
{

public:
	MaterialsLibrary();
	virtual ~MaterialsLibrary();

	void Load();
};

