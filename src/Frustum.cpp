#include "stdafx.h"
#include "Frustum.h"


Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::SetPoints(std::vector<glm::vec3>& points)
{
	mPoints.clear();
	mPoints.insert(mPoints.end(), points.begin(), points.end());
}

void Frustum::FillWithPointsXZ(std::vector<glm::vec2>& points) const
{
	//the points for the xZ plane first 0, 1, 4, 5
	points.push_back(glm::vec2(mPoints[0].x, mPoints[0].z));
	points.push_back(glm::vec2(mPoints[1].x, mPoints[1].z));
	points.push_back(glm::vec2(mPoints[4].x, mPoints[4].z));
	points.push_back(glm::vec2(mPoints[5].x, mPoints[5].z));
}

bool Frustum::Intersect2D(const std::vector<glm::vec2>& points) const 
{
	std::vector<glm::vec2> pointsFrustum;
	std::vector<glm::vec2> axis1, axis2;
	FillWithPointsXZ(pointsFrustum);
	FillWithAxis(points, axis1);
	FillWithAxis(pointsFrustum, axis2);

	axis1.insert(axis1.end(), axis2.begin(), axis2.end());

	for (glm::vec2 axis : axis1)
	{
		glm::vec2 projection1 = Projection(points, axis);
		glm::vec2 projection2 = Projection(pointsFrustum, axis);

		if (!Overlap(projection1, projection2))
		{
			return false;
		}
	}
	return true;
}

void Frustum::FillWithAxis(const std::vector<glm::vec2>& points, std::vector<glm::vec2>& axis) const
{
	axis.clear();

	for (unsigned int i = 0; i < points.size(); ++i)
	{
		glm::vec2 p1 = points[i];
		glm::vec2 p2 = points[(i + 1) % points.size()];

		glm::vec2 edge = p2 - p1;
		glm::vec2 normal(-edge.y, edge.x);

		axis.push_back(normal);
	}
}

glm::vec2 Frustum::Projection(const std::vector<glm::vec2>& points, glm::vec2& axis) const
{
	float min = glm::dot(axis, points[0]);
	float max = min;

	for (unsigned int i = 1; i < points.size(); ++i)
	{
		float value = glm::dot(axis, points[i]);
		min = glm::min(min, value);
		max = glm::max(max, value);
	}

	return glm::vec2(min, max);
}

bool Frustum::Overlap(const glm::vec2& p1, const glm::vec2& p2) const
{
	return (p1.y >= p2.x && p2.y >= p1.x);
}
