#include "WorldManager.h"

#include "Game.h"
#include <algorithm>

float squaredDistance(const glm::vec2& p1, const glm::vec2& p2) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return dx * dx + dy * dy;
}


WorldManager::WorldManager() : chunkGenerator(this), chunkProvider(this) {

}
WorldManager::~WorldManager() {

}

void WorldManager::ChunksStart(int CenterX, int CenterZ) {
	//generate new chunks that aren't in render distance.
	UpdateChunks(CenterX, CenterZ);
	UpdateLODs(CenterX, CenterZ, 1);
	UpdateLODs(CenterX, CenterZ, 2);
	UpdateLODs(CenterX, CenterZ, 3);
	UpdateLODs(CenterX, CenterZ, 4);
}
void WorldManager::UpdateChunks(int CenterX, int CenterZ) {
	/*for (auto it = chunks.begin(); it != chunks.end(); ) {
		const glm::ivec2& pos = it->first;

		int dx = pos.x - CenterX;
		int dz = pos.y - CenterZ;

		if (dx > Game::Radius_LOD0 || dx <= -Game::Radius_LOD0 || dz > Game::Radius_LOD0 || dz <= -Game::Radius_LOD0) {
			Chunk* c = it->second;
			c->DeleteMeshObjects();
			delete c;
			it = chunks.erase(it);
		}
		else {
			++it;
		}
	}*/

	//generate new chunks that aren't in render distance.
	for (int r = 0; r <= Game::RenderDistance; ++r) {
		for (int dx = -r; dx <= r; ++dx) {
			for (int dz = -r; dz <= r; ++dz) {
				if (dx != r && dz != r && dx != -r && dz != -r) continue;

				chunkProvider.ProvideChunk(CenterX + dx, CenterZ + dz);
			}
		}
	}
}
void WorldManager::UpdateLODs(int CenterLODX, int CenterLODZ, uint8_t LOD) {
	// Generate new LOD chunks at the edges
	for (int r = 0; r <= Game::RenderDistance; ++r) {
		for (int dx = -r; dx <= r; ++dx) {
			for (int dz = -r; dz <= r; ++dz) {
				if (dx != r && dz != r && dx != -r && dz != -r) continue;

				chunkProvider.ProvideLOD(CenterLODX + dx, CenterLODZ + dz, LOD);
			}
		}
	}
}
void WorldManager::WorldUpdate(float DeltaTime) {
	for (auto& pair : chunkProvider.GetAllChunks()) {
		Chunk* c = pair.second;

		if (c->IsGenerated) {
			chunkProvider.PopChunk(c);
		}
		if (c->IsModified && chunkProvider.HasAllNeighbors(c)) {
			c->IsModified = false; //so that it wont queue the chunk twice, it can cause a race condition
			chunkProvider.MeshChunk(c);
		}
		if (c->IsMeshed) {
			c->ChunkUpload();
			c->IsMeshed = false;
			c->IsRenderReady = true;
		}
	}


	for (int i = 1; i <= 4; i++) {
		for (auto& pair : chunkProvider.GetAllLODs(i)) {
			SuperChunk* c = pair.second;

			if (c->IsModified) {
				c->IsModified = false;
				chunkProvider.MeshLOD(c);
			}
			if (c->IsMeshed) {
				c->ChunkUpload();
				c->IsMeshed = false;
				c->IsRenderReady = true;
			}
		}
	}
}

bool IsChunkInRenderDistance(Chunk* c) {
	glm::ivec2 PlayerCoords = Game::player.GetCurrentChunkCoords();

	int x = std::abs(c->ChunkX - PlayerCoords.x);
	int z = std::abs(c->ChunkZ - PlayerCoords.y);

	if (x <= Game::RenderDistance && z <= Game::RenderDistance) return true;
	return false;
}
bool IsLODInRenderDistance(SuperChunk* c) {
	glm::ivec2 PlayerCoords = Game::player.GetCurrentChunkCoords(); //the coords of the player in LOD0 space
	
	//converting the player's coords from LOD0 to the coords of the LOD
	int LOD_X = PlayerCoords.x / GetLODSize(c->LOD);
	int LOD_Z = PlayerCoords.y / GetLODSize(c->LOD);

	int x = std::abs(c->ChunkX - LOD_X);
	int z = std::abs(c->ChunkZ - LOD_Z);

	if ((x * 2 > Game::RenderDistance - 1 || z * 2 > Game::RenderDistance - 1) && 
		x <= Game::RenderDistance && z <= Game::RenderDistance) return true;
	return false;
}
void WorldManager::RenderWorld() {
	Game::e_LoadedTextures[0]->bind(); //binds the texture atlas just before drawing

	for (auto& pair : chunkProvider.GetAllChunks()) {
		Chunk* c = pair.second;
		if (IsChunkInRenderDistance(c)) {
			if (c->IsRenderReady) {
				c->RenderOpaque();
				c->RenderPlants();
				c->RenderWater();
			}
		}
	}

	for (int i = 1; i <= 4; i++) {
		for (auto& pair : chunkProvider.GetAllLODs(i)) {
			SuperChunk* c = pair.second;

			if (IsLODInRenderDistance(c)) {
				if (c->IsRenderReady) {
					c->Render();
				}
			}
		}
	}
}

ChunkGenerator& WorldManager::GetChunkGenerator() {
	return this->chunkGenerator;
}
ChunkProvider& WorldManager::GetChunkProvider() {
	return chunkProvider;
}
BlockType WorldManager::getBlockAt(int x, int y, int z) { //coordinates are in world space
	Chunk* chunk = chunkProvider.ProvideChunk(x / 16, z / 16);
	return chunk->m_Blocks[IndexAt(x % 16, y, z % 16)];
}
void WorldManager::setBlockAt(int x, int y, int z, BlockType type) {
	Chunk* chunk = chunkProvider.ProvideChunk(x / 16, z / 16);
	chunk->m_Blocks[IndexAt(x % 16, y, z % 16)] = type;
}
int WorldManager::getHeightValue(int x, int z) {
	return chunkProvider.ProvideChunk(x / 16, z / 16)->GetHeightValue(x % 16, z % 16);
}
bool WorldManager::IsSolidBlock(int x, int y, int z) { //coordinates are in world space
	BlockType type = chunkProvider.ProvideChunk(x / 16, z / 16)->m_Blocks[IndexAt(x % 16, y, z % 16)];
	return Game::e_BlockRegistery[type].visibility == Opaque && type != BlockType::Air;
}
void WorldManager::PlaceBlock(int x, int y, int z, BlockType type) { //coordinates are in world space
	setBlockAt(x, y, z, type);
	Chunk* c = chunkProvider.ProvideChunk(x / 16, z / 16);
	c->IsModified = true;
}
void WorldManager::BreakBlock(int x, int y, int z) {
	setBlockAt(x, y, z, BlockType::Air);
	Chunk* c = chunkProvider.ProvideChunk(x / 16, z / 16);
	c->IsModified = true;
}