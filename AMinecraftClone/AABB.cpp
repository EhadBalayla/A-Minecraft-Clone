#include "AABB.h"
#include "Game.h"

bool AABBHelper::AABBIntersect(const AABB& a, const AABB& b) {
    return (a.max.x > b.min.x && a.min.x < b.max.x) &&
        (a.max.y > b.min.y && a.min.y < b.max.y) &&
        (a.max.z > b.min.z && a.min.z < b.max.z);
}

bool AABBHelper::RayIntersectAABB(Ray& ray, AABB& box) {
    return true;
}

void AABBHelper::UpdateAABB(AABB& box, const glm::vec3& pos, float xzDistance, float yDistance) {
    box.min = glm::vec3(pos.x - xzDistance, pos.y - yDistance, pos.z - xzDistance);
    box.max = glm::vec3(pos.x + xzDistance, pos.y + yDistance, pos.z + xzDistance);
}

glm::vec3 AABBHelper::ResolveCollision(const AABB& playerAABB, const AABB& blockAABB) {
    glm::vec3 resolution(0.0f);

    float xOverlap = std::min(playerAABB.max.x, blockAABB.max.x) - std::max(playerAABB.min.x, blockAABB.min.x);
    float yOverlap = std::min(playerAABB.max.y, blockAABB.max.y) - std::max(playerAABB.min.y, blockAABB.min.y);
    float zOverlap = std::min(playerAABB.max.z, blockAABB.max.z) - std::max(playerAABB.min.z, blockAABB.min.z);

    if (xOverlap < yOverlap && xOverlap < zOverlap) {
        resolution.x = (playerAABB.center().x < blockAABB.center().x) ? -xOverlap : xOverlap;
    }
    else if (yOverlap < zOverlap) {
        resolution.y = (playerAABB.center().y < blockAABB.center().y) ? -yOverlap : yOverlap;
    }
    else {
        resolution.z = (playerAABB.center().z < blockAABB.center().z) ? -zOverlap : zOverlap;
    }

    return resolution;
}