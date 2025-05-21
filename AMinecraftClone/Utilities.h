#pragma once
#include "Chunk.h"
#include <vector>

struct PositionedBlock { //a simple struct that returns the positions
	Block* block;
	glm::ivec3 pos;
};

struct RayHitReturnParams {
	Block* HitBlock;
	Face HitFace;
};

namespace Utils {
	std::vector<PositionedBlock> GetNearbySolidBlocks(glm::ivec3 pos, int tallness = 1); //used for when entities test collision
	Ray shootRay(glm::vec3 origin, glm::vec3 direction, float distance);
	RayHitReturnParams RayHitBlock(Ray& ray, float steps = 0.05f); //for simple stuff like block detection the ray doesn't need to be accurate, but for accurate block detection such as returning which face hit then yeah the parameter is there
}