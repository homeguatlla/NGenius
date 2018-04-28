#pragma once
#include <glm/glm.hpp>

class AABB
{
	glm::vec3 mMin;
	glm::vec3 mMax;

public:
	AABB();
	AABB(const glm::vec3& min, const glm::vec3& max);
	~AABB();

	void SetVertexMinMax(const glm::vec3& min, const glm::vec3& max);
	const glm::vec3& GetVertexMin() const;
	const glm::vec3& GetVertexMax() const;
	const glm::vec3 GetCenter() const;
	const glm::vec3 GetSize() const;
	bool Contains(const glm::vec3& point) const;
	const AABB Merge(const AABB& aabb) const;
};

