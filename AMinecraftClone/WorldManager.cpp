#include "WorldManager.h"

#include "Game.h"
#include <algorithm>

bool ShouldGenerateLODChunk(int lodX, int lodZ, uint8_t LOD, int playerChunkX, int playerChunkZ) {
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
}
float squaredDistance(const glm::vec2& p1, const glm::vec2& p2) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return dx * dx + dy * dy;
}


WorldManager::WorldManager() {
	chunkGenerator.owningWorld = this;

	running = true;
	chunkThread = std::thread(&WorldManager::ChunkThreadLoop, this);
	chunkMeshesThread = std::thread(&WorldManager::ChunkMeshesThreadLoop, this);

	for (int i = 0; i < 4; i++) {
		superChunkMeshesThread[i] = std::thread(&WorldManager::LODMeshThreadLoop, this);
		superChunkThread[i] = std::thread(&WorldManager::LODThreadLoop, this);
	}
}
WorldManager::~WorldManager() {
	running = false;
	cv.notify_all();
	meshCV.notify_all();

	chunkThread.join();
	chunkMeshesThread.join();

	superCV.notify_all();
	superMeshCV.notify_all();

	for (int i = 0; i < 4; i++) {
		superChunkMeshesThread[i].join();
		superChunkThread[i].join();
	}
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
	for (auto it = chunks.begin(); it != chunks.end(); ) {
		const glm::ivec2& pos = it->first;

		int dx = pos.x - CenterX;
		int dz = pos.y - CenterZ;

		if (dx > Game::Radius_LOD0 || dx <= -Game::Radius_LOD0 || dz > Game::Radius_LOD0 || dz <= -Game::Radius_LOD0) {
			it->second->DeleteMeshObjects();
			delete it->second;
			it = chunks.erase(it);
		}
		else {
			++it;
		}
	}

	//generate new chunks that aren't in render distance.
	for (int r = 0; r <= Game::Radius_LOD0; ++r) {
		for (int dx = -r + 1; dx <= r; ++dx) {
			for (int dz = -r + 1; dz <= r; ++dz) {
				if (dx != r && dz != r && dx != -r + 1 && dz != -r + 1) continue;
				if (chunks.find(glm::ivec2(CenterX + dx, CenterZ + dz)) == chunks.end()) {
					std::lock_guard<std::mutex> lock(chunkMutex);
					chunkGenQueue.push(glm::ivec2(CenterX + dx, CenterZ + dz));
					cv.notify_one();
				}
			}
		}
	}
}
void WorldManager::UpdateLODs(int CenterLODX, int CenterLODZ, uint8_t LOD) {
	uint8_t Radius = LOD == 1 ? Game::Radius_LOD1 : LOD == 2 ? Game::Radius_LOD2 : LOD == 3 ? Game::Radius_LOD3 : Game::Radius_LOD4;
	if (Radius == 0) return;
	
	std::unordered_map<glm::ivec2, SuperChunk*>& MAP = LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;

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
				if (MAP.find(newPos) == MAP.end()) {
					std::lock_guard<std::mutex> lock(superChunkMutex);
					superChunkGenQueue.push({newPos, LOD});
					superCV.notify_one();
				}
			}
		}
	}
}
void WorldManager::WorldUpdate(float DeltaTime) {
	{
		std::lock_guard<std::mutex> lock(finalMutex);
		while (!chunkMeshFinalQueue.empty()) {
			ChunkReady chunk = chunkMeshFinalQueue.front();
			chunkMeshFinalQueue.pop();

			Chunk* TheChunk = chunk.chunkPos;
			TheChunk->CreateMeshObjects();
			TheChunk->ChunkUpload(chunk.meshData);

			TheChunk->RenderReady = true;
			chunks[glm::ivec2(TheChunk->ChunkX, TheChunk->ChunkZ)] = TheChunk;
		}
	}

	{
		std::lock_guard<std::mutex> lock(superFinalMutex);
		while (!superChunkMeshFinalQueue.empty()) {
			SuperChunkReady chunk = std::move(superChunkMeshFinalQueue.front());
			superChunkMeshFinalQueue.pop();

			SuperChunk* TheChunk = chunk.chunk;

			TheChunk->CreateMeshObjects();
			TheChunk->ChunkUpload(chunk.meshData);

			TheChunk->RenderReady = true;

			uint8_t LOD = TheChunk->LOD;
			std::unordered_map<glm::ivec2, SuperChunk*>& MAP = LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;

			MAP[TheChunk->Pos] = TheChunk;
		}
	}
}

void WorldManager::RenderWorld() {
	//render opaque and also collect those with water
	std::vector<Chunk*> waterChunks;
	for (auto& pair : chunks) {
		if (pair.second->RenderReady) {
			pair.second->RenderOpaqueAndPlants();
			if (pair.second->HasWater)
				waterChunks.push_back(pair.second);
		}
	}
	
	//sorts chunks with water in their order
	std::sort(waterChunks.begin(), waterChunks.end(), [&](Chunk* a, Chunk* b) {
		float distA = squaredDistance(glm::vec3(a->ChunkX + 8.0f, 0, a->ChunkZ + 8.0f), Game::player.GetPosition());
		float distB = squaredDistance(glm::vec3(b->ChunkX + 8.0f, 0, b->ChunkZ + 8.0f), Game::player.GetPosition());
		return distA > distB; // Furthest first
		});


	//rendering features for the waters
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	for (auto& n : waterChunks) {
		n->RenderWater();
	}

	for (auto& pair : LOD1) {
		if (pair.second->RenderReady)
			pair.second->Render();
	}
	for (auto& pair : LOD2) {
		if (pair.second->RenderReady)
			pair.second->Render();
	}
	for (auto& pair : LOD3) {
		if (pair.second->RenderReady)
			pair.second->Render();
	}
	for (auto& pair : LOD4) {
		if (pair.second->RenderReady)
			pair.second->Render();
	}
}



bool WorldManager::IsValidChunk(int ChunkX, int ChunkZ) { //coordinates are in chunk space
	return (chunks.find(glm::ivec2(ChunkX, ChunkZ)) != chunks.end()); //basically return true if this chunk is loaded (keyword: LOADED!!!)
}
Chunk* WorldManager::getChunkAt(int ChunkX, int ChunkZ) { //these coords are in chunk space
	if (IsValidChunk(ChunkX, ChunkZ)) //basically if it found any
		return chunks[glm::ivec2(ChunkX, ChunkZ)];
	return nullptr;
}
AABB WorldManager::getBlockHitbox(int x, int y, int z) { //coordinates are in world space
	return Block::blockHitbox.MovedTo(glm::ivec3(x, y, z));
}
BlockType WorldManager::getBlockAt(int x, int y, int z) { //coordinates are in world space
	Chunk* chunk = getChunkAt(x / 16, z / 16);
	if (chunk) {
		return chunk->m_Blocks[IndexAt(x % 16, y, z % 16)];
	}
	return BlockType::Air;
}
BlockType WorldManager::getBlockAtSafe(int x, int y, int z) {
	if (y < 0 || y >= 128) return BlockType::Air;
	int chunkX = x / 16;
	int chunkZ = z / 16;
	if (!IsValidChunk(chunkX, chunkZ)) return BlockType::Air;
	Chunk* chunk = getChunkAt(chunkX, chunkZ);
	int localX = x & 15;
	int localZ = z & 15;
	return chunk->m_Blocks[IndexAt(localX, y, localZ)];
}
void WorldManager::setBlockAt(int x, int y, int z, BlockType type) {
	Chunk* chunk = getChunkAt(x / 16, z / 16);
	if (!chunk)
		return;
	chunk->m_Blocks[IndexAt(x % 16, y, z % 16)] = type;
}
bool WorldManager::IsSolidBlock(int x, int y, int z) { //coordinates are in world space
	BlockType block = getBlockAt(x, y, z);
	if (block)
		return block != BlockType::Air && Game::e_BlockRegistery[block].visibility == Opaque;
	return false;
}
void WorldManager::PlaceBlock(int x, int y, int z, BlockType type) { //coordinates are in world space
	if (!IsSolidBlock(x, y, z)) {
		setBlockAt(x, y, z, type);
		getChunkAt(x / 16, z / 16)->UpdateMesh();
	}
}
void WorldManager::BreakBlock(int x, int y, int z) {
	Chunk* chunk = getChunkAt(x / 16, z / 16);
	if (!chunk)
		return;
	chunk->m_Blocks[IndexAt(x % 16, y, z % 16)] = BlockType::Air;
	getChunkAt(x / 16, z / 16)->UpdateMesh();
}
int WorldManager::getHeightValue(int x, int z) { //coordinates are in world space
	int chunkX = floor((float)x / 16.0f);
	int chunkZ = floor((float)z / 16.0f);

	if (!IsValidChunk(chunkX, chunkZ))
		return 0;

	Chunk* chunk = getChunkAt(chunkX, chunkZ);

	int localX = x % 16;
	int localZ = z % 16;
	if (localX < 0) localX += 16;
	if (localZ < 0) localZ += 16;

	for (int y = 127; y >= 1; y--) {
		if (chunk->m_Blocks[IndexAt(localX, y, localZ)] != BlockType::Air)
			return y;
	}

	return 0;
}



Chunk* WorldManager::LoadNewChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	Chunk* chunk = new Chunk();
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningWorld = this;

	chunkGenerator.GenerateChunk2(chunk->m_Blocks, ChunkX, ChunkZ, LOD);

	return chunk;
}
SuperChunkPrep WorldManager::PrepSuperChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	BlockType* voxelData = new BlockType[VOXEL_ARRAY_SIZE]{BlockType::Air};

	chunkGenerator.GenerateChunk2(voxelData, ChunkX, ChunkZ, LOD);

	return { voxelData, glm::ivec2(ChunkX, ChunkZ), LOD };
}
bool WorldManager::IsChunkNeighboorsGood(int ChunkX, int ChunkZ) {
	if (chunks.find(glm::ivec2(ChunkX, ChunkZ)) == chunks.end())
		return false;

	int centerChunkX = static_cast<int>(std::floor(Game::player.GetPosition().x / 16.0));
	int centerChunkZ = static_cast<int>(std::floor(Game::player.GetPosition().z / 16.0));

	bool IsPositiveX = (ChunkX + 1 > centerChunkX + Game::Radius_LOD0) || IsValidChunk(ChunkX + 1, ChunkZ); //positive X of render distance
	bool IsNegativeX = (ChunkX - 1 < centerChunkX - Game::Radius_LOD0) || IsValidChunk(ChunkX - 1, ChunkZ); //negative X of render distance
	bool IsPositiveZ = (ChunkZ + 1 > centerChunkZ + Game::Radius_LOD0) || IsValidChunk(ChunkX, ChunkZ + 1); //positive Z of render distanec
	bool IsNegativeZ = (ChunkZ - 1 < centerChunkZ - Game::Radius_LOD0) || IsValidChunk(ChunkX, ChunkZ - 1); //negative Z of render distance


	return IsPositiveX && IsNegativeX && IsPositiveZ && IsNegativeZ;
}

//regular chunks threads
void WorldManager::ChunkThreadLoop() {
	while (running) {
		glm::ivec2 chunkPos; //the gathered value

		{
			std::unique_lock<std::mutex> lock(chunkMutex);
			cv.wait(lock, [this] { return !chunkGenQueue.empty() || !running; });

			if (!running && chunkGenQueue.empty()) break;

			chunkPos = chunkGenQueue.front();
			chunkGenQueue.pop();

		}
		
		Chunk* chunk = LoadNewChunk(chunkPos.x, chunkPos.y, 0); //the produced value

		{
			std::lock_guard<std::mutex> lock(meshMutex);
			chunkMeshGenQueue.push({ chunk });
		}
		meshCV.notify_one();
	}
}
void WorldManager::ChunkMeshesThreadLoop() {
	while (running) {
		Chunk* chunk;

		{
			std::unique_lock<std::mutex> lock(meshMutex);
			meshCV.wait(lock, [this] {return !chunkMeshGenQueue.empty() || !running; });

			if (!running && chunkMeshGenQueue.empty()) break;
			
			chunk = chunkMeshGenQueue.front();
			chunkMeshGenQueue.pop();
		}

		ChunkMeshUpload meshData = CreateChunkMeshData(*chunk);

		{
			std::lock_guard<std::mutex> lock(finalMutex);
			chunkMeshFinalQueue.push({ chunk, std::move(meshData)});
		}
	}
}

//LOD chunks threads
void WorldManager::LODThreadLoop() {
	while (running) {
		SuperChunkStart start;
		{
			std::unique_lock<std::mutex> lock(superChunkMutex);
			superCV.wait(lock, [this] {return !superChunkGenQueue.empty() || !running; });
			
			if (!running && superChunkGenQueue.empty()) break;

			start = superChunkGenQueue.front();
			superChunkGenQueue.pop();
		}

		int LODSize = GetLODSize(start.LOD);
		SuperChunkPrep chunkPrep = PrepSuperChunk(start.pos.x * LODSize, start.pos.y * LODSize, start.LOD);
		chunkPrep.pos = start.pos;

		{
			std::lock_guard<std::mutex> lock(superMeshMutex);
			superChunkMeshGenQueue.push(std::move(chunkPrep));
		}
		superMeshCV.notify_one();
	}
}
void WorldManager::LODMeshThreadLoop() {
	while (running) {
		SuperChunkPrep chunkPrep;

		{
			std::unique_lock<std::mutex> lock(superMeshMutex);
			superMeshCV.wait(lock, [this] {return !superChunkMeshGenQueue.empty() || !running; });

			if (!running && superChunkMeshGenQueue.empty()) break;

			chunkPrep = std::move(superChunkMeshGenQueue.front());
			superChunkMeshGenQueue.pop();
		}

		SuperChunk* chunk = new SuperChunk;
		chunk->LOD = chunkPrep.LOD;
		chunk->Pos = chunkPrep.pos;
		SuperChunkMeshUpload meshData = CreateSuperChunkMeshData(chunkPrep.voxelData, chunkPrep.LOD);

		delete[] chunkPrep.voxelData;
		{
			std::lock_guard<std::mutex> lock(superFinalMutex);
			superChunkMeshFinalQueue.push({chunk, std::move(meshData)});
		}
	}
}