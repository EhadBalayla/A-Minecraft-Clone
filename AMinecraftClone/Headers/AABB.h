#pragma once
#include "glm/glm.hpp"

struct AABB {
	glm::vec3 min;
	glm::vec3 max;


	AABB MovedTo(glm::vec3 pos) { //returns a modified AABB that is moved to a certain position
		return { min + pos, max + pos };
	}
};

struct Ray {
	glm::vec3 RayOrigin;
	glm::vec3 RayDirection;
	float RayDistance;
};

namespace AABBHelper {
	bool Intersects(const AABB& a, const AABB& b);
}

