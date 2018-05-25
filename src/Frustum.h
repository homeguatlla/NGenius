#pragma once
#include "../glm/glm/glm.hpp"
#include <vector>

class Frustum
{
	std::vector<glm::vec3> mPoints;

public:
	Frustum();
	~Frustum();
	void SetPoints(std::vector<glm::vec3>& points);
	bool Intersect2D(const std::vector<glm::vec2>& points) const;

private:
	void FillWithPointsXZ(std::vector<glm::vec2>& points) const;
	void FillWithAxis(const std::vector<glm::vec2>& points, std::vector<glm::vec2>& axis) const;
	glm::vec2 Frustum::Projection(const std::vector<glm::vec2>& points, glm::vec2& axis) const;
	bool Overlap(const glm::vec2& p1, const glm::vec2& p2) const;
};

