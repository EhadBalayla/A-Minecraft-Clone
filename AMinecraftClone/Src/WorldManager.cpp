#include "WorldManager.h"

#include "Game.h"
#include <algorithm>

#include "Utilities.h"

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
	for (int i = 0; i < Game::LODCount; i++) {
		UpdateChunks(CenterX, CenterZ, i);
	}
}
void WorldManager::UpdateChunks(int CenterX, int CenterZ, uint8_t LOD) {
	//generate new chunks that aren't in render distance.
	for (int r = 0; r <= Game::RenderDistance; ++r) {
		for (int dx = -r; dx <= r; ++dx) {
			for (int dz = -r; dz <= r; ++dz) {
				if (dx != r && dz != r && dx != -r && dz != -r) continue;

				chunkProvider.ProvideChunk(CenterX + dx, CenterZ + dz, LOD);
			}
		}
	}
}
void WorldManager::WorldUpdate(float DeltaTime) {
	chunkProvider.FlushDeletionQueue();

	for (int i = 0; i <= 4; i++) {
		for (auto& pair : chunkProvider.GetAllChunks(i)) {
			Chunk* c = pair.second;

			if (c->IsGenerated && !c->IsPopulated) {
				chunkProvider.PopChunk(c);
			}
			if (c->IsModified) {
				c->IsModified = false;
				chunkProvider.MeshChunk(c);
			}
			if (c->IsMeshed) {
				c->ChunkUpload();
				c->IsMeshed = false;
				c->IsRenderReady = true;
			}
		}
	}
}

bool IsLODInRenderDistance(Chunk* c) { //specifically for LOD chunks to check if they are in the are of the previous LOD level
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
	Game::terrainAtlas.bind();

	for (auto& pair : chunkProvider.GetAllChunks(0)) {
		Chunk* c = pair.second;
		if (IsChunkInRenderDistance(c)) {
			if (c->IsRenderReady) {
				if(c->HasOpaque) c->RenderOpaque();
				if (c->HasPlant) c->RenderPlants();
				if (c->HasWater) c->RenderWater();
				if (c->HasTransparent) c->RenderTransparent();
			}
		}
		else {
			chunkProvider.QueueChunkForDeletion(c);
		}
	}

	for (int i = 1; i <= 4; i++) {
		for (auto& pair : chunkProvider.GetAllChunks(i)) {
			Chunk* c = pair.second;

			if (IsLODInRenderDistance(c)) {
				if (c->IsRenderReady) {
					if (c->HasOpaque) c->RenderOpaque();
					if (c->HasPlant) c->RenderPlants();
					if (c->HasWater) c->RenderWater();
					if (c->HasTransparent) c->RenderTransparent();
				}
			}
			if (!IsChunkInRenderDistance(c)) {
				chunkProvider.QueueChunkForDeletion(c);
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

//coords converter helpers
inline int WorldToChunk(int a) {
	return a >= 0 ? a / 16 : (a - 15) / 16;
}
inline int WorldToBlock(int a) {
	return (a % 16 + 16) % 16;
}
BlockType WorldManager::getBlockAt(int x, int y, int z) { //coordinates are in world space
	if (y > Chunk_Height - 1 || y < 0) return BlockType::Air;

	Chunk* chunk = chunkProvider.ProvideChunk(WorldToChunk(x), WorldToChunk(z), 0);
	return chunk->m_Blocks[IndexAt(WorldToBlock(x), y, WorldToBlock(z))];
}
void WorldManager::setBlockAt(int x, int y, int z, BlockType type) {
	if (y > Chunk_Height - 1 || y < 0) return;

	Chunk* chunk = chunkProvider.ProvideChunk(WorldToChunk(x), WorldToChunk(z), 0);
	chunk->m_Blocks[IndexAt(WorldToBlock(x), y, WorldToBlock(z))] = type;
}
BlockType WorldManager::getBlockAtLOD(int x, int y, int z, uint8_t LOD) {
	if (y > Chunk_Height - 1 || y < 0) return BlockType::Air;
	
	int LODFactor = GetLODSize(LOD);

	int chunkSize = 16 * LODFactor;
	int relativeX = (x % chunkSize + chunkSize) % chunkSize;
	int relativeZ = (z % chunkSize + chunkSize) % chunkSize;

	//if (relativeX % LODFactor != 0 || relativeZ % LODFactor != 0 || y % LODFactor != 0) return BlockType::Air;

	int cx = (x < 0 ? (x - (chunkSize - 1)) : x) / chunkSize;
	int cz = (z < 0 ? (z - (chunkSize - 1)) : z) / chunkSize;

	Chunk* c = chunkProvider.ProvideChunk(cx, cz, LOD);
	return c->m_Blocks[IndexAt(relativeX / LODFactor, y, relativeZ / LODFactor)];
}
void WorldManager::setBlockAtLOD(int x, int y, int z, BlockType type, uint8_t LOD) {
	if (y > Chunk_Height - 1 || y < 0) return;

	int LODFactor = GetLODSize(LOD);

	int chunkSize = 16 * LODFactor;
	int relativeX = (x % chunkSize + chunkSize) % chunkSize;
	int relativeZ = (z % chunkSize + chunkSize) % chunkSize;

	if (relativeX % LODFactor != 0 || relativeZ % LODFactor != 0 || y % LODFactor != 0) return;

	int cx = (x < 0 ? (x - (chunkSize - 1)) : x) / chunkSize;
	int cz = (z < 0 ? (z - (chunkSize - 1)) : z) / chunkSize;

	Chunk* c = chunkProvider.ProvideChunk(cx, cz, LOD);
	c->m_Blocks[IndexAt(relativeX / LODFactor, y, relativeZ / LODFactor)] = type;
}
int WorldManager::getHeightValue(int x, int z) {
	return chunkProvider.ProvideChunk(x / 16, z / 16, 0)->GetHeightValue(x % 16, z % 16);
}
bool WorldManager::IsSolidBlock(int x, int y, int z) { //coordinates are in world space
	BlockType type = getBlockAt(x, y, z);
	return (Game::e_BlockRegistery[type].visibility == BlockVisiblity::Opaque || Game::e_BlockRegistery[type].visibility == BlockVisiblity::Transparent) && type != BlockType::Air;
}
void WorldManager::PlaceBlock(int x, int y, int z, BlockType type) { //coordinates are in world space
	setBlockAt(x, y, z, type);
	Chunk* c = chunkProvider.ProvideChunk(WorldToChunk(x), WorldToChunk(z), 0);
	c->IsModified = true;
}
void WorldManager::BreakBlock(int x, int y, int z) {
	setBlockAt(x, y, z, BlockType::Air);
	Chunk* c = chunkProvider.ProvideChunk(WorldToChunk(x), WorldToChunk(z), 0);
	c->IsModified = true;
}