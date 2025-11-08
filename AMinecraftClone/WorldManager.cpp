#include "WorldManager.h"

#include "Game.h"
#include <algorithm>

/*bool ShouldGenerateLODChunk(int lodX, int lodZ, uint8_t LOD, int playerChunkX, int playerChunkZ) {
	int lowerLOD = LOD - 1;
	int LODSize = GetLODSize(LOD);
	int lowerLODSize = GetLODSize(lowerLOD); // size in chunks of lower LOD

	int radiusLowerLOD = LOD == 1 ? Game::Radius_LOD0 : LOD == 2 ? Game::Radius_LOD1 : 
		LOD == 3 ? Game::Radius_LOD2 : Game::Radius_LOD3;

	// Convert candidate LOD chunk center to lower LOD chunk space
	int centerXInLowerLOD = lodX * LODSize / lowerLODSize;
	int centerZInLowerLOD = lodZ * LODSize / lowerLODSize;

	int dx = centerXInLowerLOD - playerChunkX / lowerLODSize;
	int dz = centerZInLowerLOD - playerChunkZ / lowerLODSize;

	return dx < -radiusLowerLOD + 1 || dz < -radiusLowerLOD + 1 || dx >= radiusLowerLOD || dz >= radiusLowerLOD;
}*/
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

				Chunk* c = chunkProvider.ProvideChunk(CenterX + dx, CenterZ + dz);
			}
		}
	}
}
void WorldManager::UpdateLODs(int CenterLODX, int CenterLODZ, uint8_t LOD) {
	/*uint8_t Radius = LOD == 1 ? Game::Radius_LOD1 : LOD == 2 ? Game::Radius_LOD2 : LOD == 3 ? Game::Radius_LOD3 : Game::Radius_LOD4;
	if (Radius == 0) return;
	
	std::unordered_map<glm::ivec2, SuperChunk*>& MAP = LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;
	std::unordered_set<glm::ivec2>& Queued = LOD == 1 ? LOD1QueuedChunks : LOD == 2 ? LOD2QueuedChunks : LOD == 3 ? LOD3QueuedChunks : LOD4QueuedChunks;

	// Delete far-away LOD chunks
	for (auto it = MAP.begin(); it != MAP.end(); ) {
		const glm::ivec2& pos = it->first;

		int dx = pos.x - CenterLODX;
		int dz = pos.y - CenterLODZ;

		if (!ShouldGenerateLODChunk(CenterLODX + dx, CenterLODZ + dz, LOD,
			Game::player.GetCurrentChunkCoords().x, Game::player.GetCurrentChunkCoords().y) || 
			dx > Radius || dx <= -Radius || dz > Radius || dz <= -Radius) {
			it->second->DeleteMeshObjects();
			delete it->second;
			it = MAP.erase(it);
		}
		else {
			++it;
		}
	}

	// Generate new LOD chunks at the edges
	for (int r = 0; r <= Radius; ++r) {
		for (int dx = -r + 1; dx <= r; ++dx) {
			for (int dz = -r + 1; dz <= r; ++dz) {
				if (dx != r && dx != -r + 1 && dz != r && dz != -r + 1) continue; // only edges
				
				if (!ShouldGenerateLODChunk(CenterLODX + dx, CenterLODZ + dz, LOD, 
					Game::player.GetCurrentChunkCoords().x, Game::player.GetCurrentChunkCoords().y)) continue;

				glm::ivec2 newPos(CenterLODX + dx, CenterLODZ + dz);
				if (MAP.find(newPos) == MAP.end() && Queued.find(newPos) == Queued.end()) {

					Queued.insert(newPos);
					{
						std::lock_guard<std::mutex> lock(superChunkMutex);
						superChunkGenQueue.push({ newPos, LOD });
					}
					superCV.notify_one();
				}
			}
		}
	}*/
}
void WorldManager::WorldUpdate(float DeltaTime) {
	/* {
		std::unique_lock<std::mutex> lock(finalMutex);
		while (!chunkMeshFinalQueue.empty()) {
			ChunkReady& chunk = chunkMeshFinalQueue.front();
			lock.unlock();

			Chunk* TheChunk = chunk.chunkPos;
			TheChunk->CreateMeshObjects();
			TheChunk->ChunkUpload(chunk.meshData);
			TheChunk->RenderReady = true;

			chunks[glm::ivec2(TheChunk->ChunkX, TheChunk->ChunkZ)] = TheChunk;
			queuedChunks.erase(glm::ivec2(TheChunk->ChunkX, TheChunk->ChunkZ));

			lock.lock();
			chunkMeshFinalQueue.pop();
		}
	}

	{
		std::unique_lock<std::mutex> lock(superFinalMutex);
		while (!superChunkMeshFinalQueue.empty()) {
			SuperChunkReady& chunk = superChunkMeshFinalQueue.front();
			lock.unlock();

			SuperChunk* TheChunk = chunk.chunk;
			TheChunk->CreateMeshObjects();
			TheChunk->ChunkUpload(chunk.meshData);
			TheChunk->RenderReady = true;


			uint8_t LOD = TheChunk->LOD;
			std::unordered_map<glm::ivec2, SuperChunk*>& MAP = LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;
			std::unordered_set<glm::ivec2>& Queued = LOD == 1 ? LOD1QueuedChunks : LOD == 2 ? LOD2QueuedChunks : LOD == 3 ? LOD3QueuedChunks : LOD4QueuedChunks;

			MAP[TheChunk->Pos] = TheChunk;
			Queued.erase(TheChunk->Pos);

			lock.lock();
			superChunkMeshFinalQueue.pop();
		}
	}*/
}

bool IsChunkInRenderDistance(Chunk* c) {
	glm::ivec2 PlayerCoords = Game::player.GetCurrentChunkCoords();
	
	int x = std::abs(c->ChunkX - PlayerCoords.x);
	int z = std::abs(c->ChunkZ - PlayerCoords.y);
	
	if (x <= Game::RenderDistance && z <= Game::RenderDistance) return true;
	return false;
}
void WorldManager::RenderWorld() {
	for (auto& pair : chunkProvider.GetAllChunks()) {
		Chunk* c = pair.second;
		if (IsChunkInRenderDistance(c)) {
			if (pair.second->IsPopulated) {
				if (pair.second->IsRenderReady) {
					pair.second->RenderOpaqueAndPlants();
					pair.second->RenderWater();
				}
				else {
					pair.second->CreateMeshObjects();
					ChunkMeshUpload meshdata = CreateChunkMeshData(*pair.second);
					pair.second->ChunkUpload(meshdata);
					pair.second->IsRenderReady = true;
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

	return false;
}
void WorldManager::PlaceBlock(int x, int y, int z, BlockType type) { //coordinates are in world space

}
void WorldManager::BreakBlock(int x, int y, int z) {

}