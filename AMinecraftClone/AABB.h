#pragma once
#include "glm/glm.hpp"

struct AABB {
	glm::vec3 min;
	glm::vec3 max;


	glm::vec3 center() const { //returns the center of the AABB
		return glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	}

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
	void UpdateAABB(AABB& box ,const glm::vec3& pos, float xzDistance, float yDistance); //updates a given box using a center position as well as distances

	bool AABBIntersect(const AABB& box1, const AABB& box2);
	bool RayIntersectAABB(Ray& ray, AABB& box);

	glm::vec3 ResolveCollision(const AABB& playerAABB, const AABB& blockAABB);

}

