#include "stdafx.h"
#include "AABB.h"


AABB::AABB()
{

}

AABB::AABB(const glm::vec3& min, const glm::vec3& max) :
mMin(min),
mMax(max)
{
}

AABB::~AABB()
{
}

void AABB::SetVertexMinMax(const glm::vec3& min, const glm::vec3& max)
{
	mMin = min;
	mMax = max;
}

const glm::vec3& AABB::GetVertexMin() const
{
	return mMin;
}

const glm::vec3& AABB::GetVertexMax() const
{
	return mMax;
}


