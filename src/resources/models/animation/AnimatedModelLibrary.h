#pragma once
#include "../../BaseLibrary.h"

class AnimatedModel;

class AnimatedModelLibrary : public BaseLibrary<AnimatedModel*>
{
public:
	AnimatedModelLibrary();
	~AnimatedModelLibrary();
};

