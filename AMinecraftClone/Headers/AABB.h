#pragma once
#include "glm/glm.hpp"

struct AABB {
	glm::dvec3 min;
	glm::dvec3 max;


	AABB MovedTo(glm::dvec3 pos) { //returns a modified AABB that is moved to a certain position
		return { min + pos, max + pos };
	}
};

struct Ray {
	glm::dvec3 RayOrigin;
	glm::dvec3 RayDirection;
	double RayDistance;
};

namespace AABBHelper {
	bool Intersects(const AABB& a, const AABB& b);
}

