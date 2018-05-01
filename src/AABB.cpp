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
	return mMax - mMin;
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
