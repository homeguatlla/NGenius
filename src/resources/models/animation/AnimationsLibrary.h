#pragma once
#include "../../BaseLibrary.h"

class Animation;

class AnimationsLibrary : public BaseLibrary<Animation*>
{
public:
	AnimationsLibrary();
	~AnimationsLibrary();
};

