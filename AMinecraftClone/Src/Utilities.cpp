#include "Utilities.h"
#include "Game.h"

std::vector<PositionedBlock> Utils::GetNearbySolidBlocks(glm::i64vec3 pos, int tallness) { //basically gathers the surroning blocks around a position
	std::vector<PositionedBlock> returnal;
	if (Game::level->GetWorld().IsSolidBlock(pos.x, pos.y - 1, pos.z))
		returnal.push_back({ Game::level->GetWorld().getBlockAt(pos.x, pos.y - 1, pos.z), glm::ivec3(pos.x, pos.y - 1, pos.z) });

	for (int i = 0; i < tallness; i++) {
		if (Game::level->GetWorld().IsSolidBlock(pos.x + 1, pos.y + i, pos.z))
			returnal.push_back({ Game::level->GetWorld().getBlockAt(pos.x + 1, pos.y + i, pos.z), glm::ivec3(pos.x + 1, pos.y + i, pos.z) });
		if (Game::level->GetWorld().IsSolidBlock(pos.x - 1, pos.y + i, pos.z))
			returnal.push_back({ Game::level->GetWorld().getBlockAt(pos.x - 1, pos.y + i, pos.z), glm::ivec3(pos.x - 1, pos.y + i, pos.z) });
		if (Game::level->GetWorld().IsSolidBlock(pos.x, pos.y + i, pos.z + 1))
			returnal.push_back({ Game::level->GetWorld().getBlockAt(pos.x, pos.y + i, pos.z + 1), glm::ivec3(pos.x, pos.y + i, pos.z + 1) });
		if (Game::level->GetWorld().IsSolidBlock(pos.x, pos.y + i, pos.z - 1))
			returnal.push_back({ Game::level->GetWorld().getBlockAt(pos.x, pos.y + i, pos.z - 1), glm::ivec3(pos.x, pos.y + i, pos.z - 1) });
	}

	if (Game::level->GetWorld().IsSolidBlock(pos.x, pos.y + tallness, pos.z))
		returnal.push_back({ Game::level->GetWorld().getBlockAt(pos.x, pos.y + tallness, pos.z), glm::ivec3(pos.x, pos.y + tallness, pos.z) });

	return returnal;
}

Ray Utils::shootRay(glm::dvec3 origin, glm::dvec3 direction, float distance) {
	return { origin, direction, distance };
}

RayHitReturnParams Utils::RayHitBlock(Ray& ray, double steps) {
	for (double t = 0; t < ray.RayDistance; t += steps) {
		glm::dvec3 currentPos = ray.RayOrigin + ray.RayDirection * t;
		glm::i64vec3 blockPos = glm::floor(currentPos);
		if (Game::level->GetWorld().IsSolidBlock(blockPos.x, blockPos.y, blockPos.z)) {
			BlockType blockPoint = Game::level->GetWorld().getBlockAt(blockPos.x, blockPos.y, blockPos.z);
			
			//x y and z relative to the block itself, so we can check which face based on which is closer, however accuracy depends on the ste
			double relX = currentPos.x - blockPos.x;
			double relY = currentPos.y - blockPos.y;
			double relZ = currentPos.z - blockPos.z;
			if (relY > relX && relY > relZ) //the up face
			{
				return { blockPoint, blockPos, Face::Top };
			}
			else if (relY < relX && relY < relZ) //the down face
			{
				return { blockPoint, blockPos, Face::Bottom };
			}
			else if (relX > relY && relX > relZ) // the right face
			{
				return { blockPoint, blockPos, Face::Back };
			}
			else if (relX < relY && relX < relZ) // the left face
			{
				return { blockPoint, blockPos, Face::Front };
			}
			else if (relZ > relX && relZ > relY) // the forward face
			{
				return { blockPoint, blockPos, Face::Left };
			}
			else if (relZ < relX && relZ < relY) // the backward face
			{
				return { blockPoint, blockPos, Face::Right };
			}
		}
	}
	return {};
}







bool IsChunkInRenderDistance(Chunk* c) {
	glm::ivec2 PlayerCoords = Game::player.GetCurrentChunkCoords();

	int LOD_X = PlayerCoords.x / GetLODSize(c->LOD);
	int LOD_Z = PlayerCoords.y / GetLODSize(c->LOD);

	int x = std::abs(c->ChunkX - LOD_X);
	int z = std::abs(c->ChunkZ - LOD_Z);

	if (x <= Game::RenderDistance && z <= Game::RenderDistance) return true;
	return false;
}