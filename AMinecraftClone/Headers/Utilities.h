#pragma once
#include "Block.h"
#include <vector>

struct PositionedBlock { //a simple struct that returns the positions
	BlockType block;
	glm::i64vec3 pos;
};

namespace Utils {
	std::vector<PositionedBlock> GetNearbySolidBlocks(glm::i64vec3 pos, int tallness = 1); //used for when entities test collision
	Ray shootRay(glm::dvec3 origin, glm::dvec3 direction, float distance);
}

class Chunk;
bool IsChunkInRenderDistance(Chunk* c); //generalized for all chunks