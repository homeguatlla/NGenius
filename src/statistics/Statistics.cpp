#include "stdafx.h"
#include "statistics.h"
#include "../resources/systems/renderSystem/RenderSystem.h"

#include <iostream>

Statistics::Statistics()
{
}

Statistics::~Statistics()
{
}

void Statistics::Visit(RenderSystem& renderSystem)
{
	std::cout << "rendersystem";
}