#include "WorldManager.h"

#include "Game.h"
#include <algorithm>

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

	superChunkThread = std::thread(&WorldManager::LODThreadLoop, this);
	superChunkMeshesThread = std::thread(&WorldManager::LODMeshThreadLoop, this);
}
WorldManager::~WorldManager() {
	running = false;
	cv.notify_all();
	meshCV.notify_all();
	superCV.notify_all();
	superMeshCV.notify_all();

	chunkThread.join();
	chunkMeshesThread.join();

	superChunkThread.join();
	superChunkMeshesThread.join();
}


void WorldManager::UpdateChunks(int CenterX, int CenterZ) {
	for (auto it = chunks.begin(); it != chunks.end(); ) {
		const glm::ivec2& pos = it->first;

		int dx = std::abs(pos.x - CenterX);
		int dz = std::abs(pos.y - CenterZ);

		if (dx > Game::Radius_LOD0 || dz > Game::Radius_LOD0) {
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
		for (int dx = -r; dx <= r; ++dx) {
			for (int dz = -r; dz <= r; ++dz) {
				if (std::abs(dx) != r && std::abs(dz) != r) continue; // Only edges of the square
				if (chunks.find(glm::ivec2(CenterX + dx, CenterZ + dz)) == chunks.end()) {
					std::lock_guard<std::mutex> lock(chunkMutex);
					chunkGenQueue.push(glm::ivec2(CenterX + dx, CenterZ + dz));
					cv.notify_one();
				}
			}
		}
	}

	UpdateLOD(CenterX, CenterZ, 1);
	UpdateLOD(CenterX, CenterZ, 2);
	UpdateLOD(CenterX, CenterZ, 3);
	UpdateLOD(CenterX, CenterZ, 4);
}
void WorldManager::UpdateLOD(int CenterX, int CenterZ, uint8_t LOD) {

	std::unordered_map<glm::ivec2, SuperChunk*>& LODMap = LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;

	int step = 1;
	for (int i = 0; i < LOD; ++i)
		step *= 3;

	int Radiuses[5] = { Game::Radius_LOD0, Game::Radius_LOD0 + Game::Radius_LOD1 * 3,
	Game::Radius_LOD0 + Game::Radius_LOD1 * 3 + Game::Radius_LOD2 * 9,
		Game::Radius_LOD0 + Game::Radius_LOD1 * 3 + Game::Radius_LOD2 * 9 + Game::Radius_LOD3 * 27,
		Game::Radius_LOD0 + Game::Radius_LOD1 * 3 + Game::Radius_LOD2 * 9 + Game::Radius_LOD3 * 27 + Game::Radius_LOD4 * 81
	};

	int BeforeRadiuses[5] = { 0, Game::Radius_LOD0 ,
		Game::Radius_LOD0 + Game::Radius_LOD1 * 3,
		Game::Radius_LOD0 + Game::Radius_LOD1 * 3 + Game::Radius_LOD2 * 9,
		Game::Radius_LOD0 + Game::Radius_LOD1 * 3 + Game::Radius_LOD2 * 9 + Game::Radius_LOD3 * 27
	};


	int startX = CenterX - Radiuses[LOD];
	int endX = CenterX + Radiuses[LOD];
	int startZ = CenterZ - Radiuses[LOD];
	int endZ = CenterZ + Radiuses[LOD];

	for (int dx = startX; dx <= endX; dx += step) {
		for (int dz = startZ; dz <= endZ; dz += step) {
			if (dx >= CenterX - BeforeRadiuses[LOD] && dx <= CenterX + BeforeRadiuses[LOD] &&
				dz >= CenterZ - BeforeRadiuses[LOD] && dz <= CenterZ + BeforeRadiuses[LOD]) continue;

			glm::ivec2 superPos(dx, dz);

			if (LODMap.find(superPos) == LODMap.end()) {
				std::lock_guard<std::mutex> lock(superChunkMutex);
				superChunkGenQueue.push({ superPos, LOD });
				superCV.notify_one();
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
			LOD1[TheChunk->ChunkPos] = TheChunk;
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
	return getBlockAt(x, y, z)->blockHitbox.MovedTo(glm::ivec3(x, y, z));
}
Block* WorldManager::getBlockAt(int x, int y, int z) { //coordinates are in world space
	Chunk* chunk = getChunkAt(floor((float)x / 16.0f), floor((float)z / 16.0f));
	if (chunk) {
		return &chunk->m_Blocks[(x % 16 + 16) % 16][y][(z % 16 + 16) % 16];
	}
	return nullptr;
}
Block* WorldManager::getBlockAtSafe(int x, int y, int z) {
	if (y < 0 || y >= 128) return nullptr;
	int chunkX = x / 16;
	int chunkZ = z / 16;
	if (!IsValidChunk(chunkX, chunkZ)) return nullptr;
	Chunk* chunk = getChunkAt(chunkX, chunkZ);
	int localX = x & 15;
	int localZ = z & 15;
	return &chunk->m_Blocks[localX][y][localZ];
}
void WorldManager::setBlockAt(int x, int y, int z, BlockType type) {
	Block* block = getBlockAt(x, y, z);
	if (!block)
		return;
	block->setType(type);
}
bool WorldManager::IsSolidBlock(int x, int y, int z) { //coordinates are in world space
	Block* block = getBlockAt(x, y, z);
	if (block)
		return block->getType() != BlockType::Air && block->data.visibility == Opaque;
	return false;
}
void WorldManager::PlaceBlock(int x, int y, int z, BlockType type) { //coordinates are in world space
	if (!IsSolidBlock(x, y, z)) {
		getBlockAt(x, y, z)->setType(type);
	}
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
		if (chunk->m_Blocks[localX][y][localZ].getType() != BlockType::Air)
			return y;
	}

	return 0;
}



Chunk* WorldManager::LoadNewChunk(int ChunkX, int ChunkZ) {
	Chunk* chunk = new Chunk();
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningWorld = this;

	//chunkGenerator.GenerateChunk2(*chunk);
	chunkGenerator.GenerateChunk(*chunk);

	return chunk;
}
SuperChunkPrep WorldManager::PrepSuperChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	size_t countPerAxis[5] = { 1, 3, 9, 27, 81 };

	Chunk** chunks = new Chunk*[countPerAxis[LOD] * countPerAxis[LOD]];
	for (int x = 0; x < countPerAxis[LOD]; x++) {
		for (int z = 0; z < countPerAxis[LOD]; z++) {
			chunks[x + z * countPerAxis[LOD]] = LoadNewChunk(ChunkX + x, ChunkZ + z);
		}
	}
	return { chunks, countPerAxis[LOD] * countPerAxis[LOD], glm::ivec2(ChunkX, ChunkZ), LOD };
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
		
		Chunk* chunk = LoadNewChunk(chunkPos.x, chunkPos.y); //the produced value

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

		SuperChunkPrep chunkPrep = PrepSuperChunk(start.pos.x, start.pos.y, start.LOD);
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
		chunk->ChunkPos = chunkPrep.pos;
		SuperChunkMeshUpload meshData = CreateSuperChunkMeshData(chunkPrep.chunks, chunkPrep.chunksCount, chunkPrep.LOD);
		delete[] chunkPrep.chunks;
		{
			std::lock_guard<std::mutex> lock(superFinalMutex);
			superChunkMeshFinalQueue.push({ chunk, std::move(meshData) });
		}
	}
}