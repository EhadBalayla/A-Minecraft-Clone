#include "ChunkProvider.h"
#include "WorldManager.h"

ChunkProvider::ChunkProvider(WorldManager* world) : owningWorld(world) {
	ThreadsRunning = true;

	//start chunk threads
	ChunkGenThread = std::thread(&ChunkProvider::ChunkGen, this);
	ChunkPopThread = std::thread(&ChunkProvider::ChunkPop, this);
	ChunkMeshThread = std::thread(&ChunkProvider::ChunkMesh, this);

	//start LOD threads
	LODGenThread = std::thread(&ChunkProvider::LODGen, this);
	LODMeshThread = std::thread(&ChunkProvider::LODMesh, this);
}
ChunkProvider::~ChunkProvider() {
	ThreadsRunning = false;
	//stop LOD threads
	LODMeshCV.notify_all();
	LODGenCV.notify_all();
	LODMeshThread.join();
	LODGenThread.join();

	//stop chunk threads
	ChunkMeshCV.notify_all();
	ChunkPopCV.notify_all();
	ChunkGenCV.notify_all();
	ChunkMeshThread.join();
	ChunkPopThread.join();
	ChunkGenThread.join();
}

//the chunks public functions
Chunk* ChunkProvider::ProvideChunk(int ChunkX, int ChunkZ) {
	if (IsValidChunk(ChunkX, ChunkZ)) {
		return chunks[glm::ivec2(ChunkX, ChunkZ)];
	}
	
	//create new chunk
	Chunk* c = LoadNewChunk(ChunkX, ChunkZ);
	chunks[glm::ivec2(ChunkX, ChunkZ)] = c;
	c->CreateMeshObjects();

	{
		std::lock_guard<std::mutex> lock(ChunkGenMutex);
		ChunkGenQueue.push(c);
	}
	ChunkGenCV.notify_one();

	return c;
}
void ChunkProvider::MeshChunk(Chunk* c) {
	{
		std::lock_guard<std::mutex> lock(ChunkMeshMutex);
		ChunkMeshQueue.push(c);
	}
	ChunkMeshCV.notify_one();
}
void ChunkProvider::PopChunk(Chunk* c) {
	int ChunkX = c->ChunkX;
	int ChunkZ = c->ChunkZ;

	if (!c->IsPopulated && IsValidChunk(ChunkX + 1, ChunkZ + 1) && IsValidChunk(ChunkX, ChunkZ + 1) && IsValidChunk(ChunkX + 1, ChunkZ)) {
		populate(ChunkX, ChunkZ);
	}
}
bool ChunkProvider::HasAllNeighbors(Chunk* c) {
	int X = c->ChunkX;
	int Z = c->ChunkZ;

	return IsValidChunk(X + 1, Z) && IsValidChunk(X - 1, Z) && IsValidChunk(X, Z + 1) && IsValidChunk(X, Z - 1);
}
std::unordered_map<glm::ivec2, Chunk*>& ChunkProvider::GetAllChunks() {
	return chunks;
}


//the LODs public functions
SuperChunk* ChunkProvider::ProvideLOD(int ChunkX, int ChunkZ, uint8_t LOD) {
	std::unordered_map<glm::ivec2, SuperChunk*>& MAP = GetLODMap(LOD);
	if (IsValidLOD(ChunkX, ChunkZ, LOD)) {
		return MAP[glm::ivec2(ChunkX, ChunkZ)];
	}

	//create new LOD
	SuperChunk* c = LoadNewLOD(ChunkX, ChunkZ, LOD);
	MAP[glm::ivec2(ChunkX, ChunkZ)] = c;
	c->CreateMeshObjects();

	{
		std::lock_guard<std::mutex> lock(LODGenMutex);
		LODGenQueue.push(c);
	}
	LODGenCV.notify_one();

	return c;
}
void ChunkProvider::MeshLOD(SuperChunk* c) {
	{
		std::lock_guard<std::mutex> lock(LODMeshMutex);
		LODMeshQueue.push(c);
	}
	LODMeshCV.notify_one();
}
std::unordered_map<glm::ivec2, SuperChunk*>& ChunkProvider::GetAllLODs(uint8_t LOD) {
	return GetLODMap(LOD);
}



//chunks checking
bool ChunkProvider::IsValidChunk(int ChunkX, int ChunkZ) {
	return chunks.find(glm::ivec2(ChunkX, ChunkZ)) != chunks.end(); //basically return true if this chunk is loaded (keyword: LOADED!!!)
}
Chunk* ChunkProvider::LoadNewChunk(int ChunkX, int ChunkZ) {
	Chunk* chunk = new Chunk();
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningWorld = owningWorld;

	return chunk;
}

//LODs checking
std::unordered_map<glm::ivec2, SuperChunk*>& ChunkProvider::GetLODMap(uint8_t LOD) {
	return LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;
}
bool ChunkProvider::IsValidLOD(int ChunkX, int ChunkZ, uint8_t LOD) {
	std::unordered_map<glm::ivec2, SuperChunk*>& MAP = GetLODMap(LOD);
	return MAP.find(glm::ivec2(ChunkX, ChunkZ)) != MAP.end(); //basically returns true if this LOD is loaded (keyword: LOADED!!!)
}
SuperChunk* ChunkProvider::LoadNewLOD(int ChunkX, int ChunkZ, uint8_t LOD) {
	SuperChunk* chunk = new SuperChunk();
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningWorld = owningWorld;
	chunk->LOD = LOD;

	return chunk;
}

void ChunkProvider::populate(int ChunkX, int ChunkZ) {
	Chunk* c = ProvideChunk(ChunkX, ChunkZ);
	if (!c->IsPopulated) {
		c->IsPopulated = true;
		{
			std::lock_guard<std::mutex> lock(ChunkPopMutex);
			ChunkPopQueue.push(c);
		}
		ChunkPopCV.notify_one();
	}
}



void ChunkProvider::ChunkGen() {
	while (ThreadsRunning) {
		Chunk* c;
		{
			std::unique_lock<std::mutex> lock(ChunkGenMutex);
			ChunkGenCV.wait(lock, [this] {return !ChunkGenQueue.empty() || !ThreadsRunning; });

			c = ChunkGenQueue.front();
			ChunkGenQueue.pop();
		}

		owningWorld->GetChunkGenerator().Gen1.GenerateChunk(c->m_Blocks, c->ChunkX, c->ChunkZ, 0);
		c->IsGenerated = true;
	}
}
void ChunkProvider::ChunkPop() {
	while (ThreadsRunning) {
		Chunk* c;
		{
			std::unique_lock<std::mutex> lock(ChunkPopMutex);
			ChunkPopCV.wait(lock, [this] {return !ChunkPopQueue.empty() || !ThreadsRunning; });

			c = ChunkPopQueue.front();
			ChunkPopQueue.pop();
		}
		owningWorld->GetChunkGenerator().Gen3.populate(c->ChunkX, c->ChunkZ);
		c->IsModified = true;
	}
}
void ChunkProvider::ChunkMesh() {
	while (ThreadsRunning) {
		Chunk* c;
		{
			std::unique_lock<std::mutex> lock(ChunkMeshMutex);
			ChunkMeshCV.wait(lock, [this] {return !ChunkMeshQueue.empty() || !ThreadsRunning; });

			c = ChunkMeshQueue.front();
			ChunkMeshQueue.pop();
		}

		c->CreateChunkMeshData();
		c->IsMeshed = true;
	}
}

void ChunkProvider::LODGen() {
	while (ThreadsRunning) {
		SuperChunk* c;
		{
			std::unique_lock<std::mutex> lock(LODGenMutex);
			LODGenCV.wait(lock, [this] {return !LODGenQueue.empty() || !ThreadsRunning; });

			c = LODGenQueue.front();
			LODGenQueue.pop();
		}
		uint8_t LODSize = GetLODSize(c->LOD);
		owningWorld->GetChunkGenerator().Gen1.GenerateChunk(c->m_Blocks, c->ChunkX * LODSize, c->ChunkZ * LODSize, c->LOD);
		c->IsGenerated = true;
		c->IsModified = true;
	}
}
void ChunkProvider::LODMesh() {
	while (ThreadsRunning) {
		SuperChunk* c;
		{
			std::unique_lock<std::mutex> lock(LODMeshMutex);
			LODMeshCV.wait(lock, [this] {return !LODMeshQueue.empty() || !ThreadsRunning; });

			c = LODMeshQueue.front();
			LODMeshQueue.pop();
		}

		c->CreateSuperChunkMeshData();
		c->IsMeshed = true;
	}
}