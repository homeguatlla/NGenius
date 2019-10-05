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

bool AABB::Contains(const glm::vec3& point) const
{
	return	point.x >= mMin.x && point.x <= mMax.x &&
			point.y >= mMin.y && point.y <= mMax.y &&
			point.z >= mMin.z && point.z <= mMax.z;
}
const glm::vec3 AABB::GetCenter() const
{
	return (mMin + mMax) * 0.5f;
}

const glm::vec3 AABB::GetSize() const
{
	return glm::abs(mMax - mMin);
}

const AABB AABB::Merge(const AABB& aabb) const
{
	glm::vec3 min(	glm::min(mMin.x, aabb.GetVertexMin().x),
					glm::min(mMin.y, aabb.GetVertexMin().y),
					glm::min(mMin.z, aabb.GetVertexMin().z));

	glm::vec3 max(	glm::max(mMax.x, aabb.GetVertexMax().x),
					glm::max(mMax.y, aabb.GetVertexMax().y),
					glm::max(mMax.z, aabb.GetVertexMax().z));

	return AABB(min, max);
}

void AABB::Expand(const glm::vec3& size)
{
	mMin -= size;
	mMax += size;
}

float AABB::GetVolume() const
{
	glm::vec3 size = GetSize();
	//this ifs are in case the aabb is a plane, or a particle
	//the volume will be the area. Let's see if this works.
	if (size.x == 0.0f)
	{
		return size.y * size.z;
	}
	else if (size.y == 0.0f)
	{
		return size.x * size.z;
	}
	else if (size.z == 0.0f)
	{
		return size.x * size.y;
	}

	return size.x * size.y * size.z;
}
