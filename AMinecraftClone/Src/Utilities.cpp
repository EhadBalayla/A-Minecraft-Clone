#include "Utilities.h"
#include "Game.h"

bool IsChunkInRenderDistance(Chunk* c) {
	glm::ivec2 PlayerCoords = Game::player.GetCurrentChunkCoords();

	int LOD_X = PlayerCoords.x / GetLODSize(c->LOD);
	int LOD_Z = PlayerCoords.y / GetLODSize(c->LOD);

	int x = std::abs(c->ChunkX - LOD_X);
	int z = std::abs(c->ChunkZ - LOD_Z);

	if (x <= Game::RenderDistance && z <= Game::RenderDistance) return true;
	return false;
}