#include "stdafx.h"
#include "Log.h"

#include <iostream>


Log::Log(LOG_TYPE type) : mType(type)
{
}

void Log::Reset()
{
	//reset
	std::cout << "\033[0m" << "";
}

Log::~Log()
{
	std::cout << mBuffer.str();
	Reset();
}
