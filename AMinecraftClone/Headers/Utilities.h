#pragma once
#include "Chunk.h"
#include <vector>

struct PositionedBlock { //a simple struct that returns the positions
	BlockType block;
	glm::i64vec3 pos;
};

struct RayHitReturnParams {
	BlockType HitBlock;
	glm::i64vec3 pos;
	Face HitFace;
};

namespace Utils {
	std::vector<PositionedBlock> GetNearbySolidBlocks(glm::i64vec3 pos, int tallness = 1); //used for when entities test collision
	Ray shootRay(glm::dvec3 origin, glm::dvec3 direction, float distance);
	RayHitReturnParams RayHitBlock(Ray& ray, double steps = 0.05f); //for simple stuff like block detection the ray doesn't need to be accurate, but for accurate block detection such as returning which face hit then yeah the parameter is there
}

bool IsChunkInRenderDistance(Chunk* c); //generalized for all chunks