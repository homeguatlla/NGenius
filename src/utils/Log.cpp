#include "stdafx.h"
#include "Log.h"

#include <iostream>


Log::Log(LOG_TYPE type) : mType(type)
{
}


Log::~Log()
{
	std::cout << mBuffer.str();
}
