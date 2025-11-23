#include "ChunkProvider.h"
#include "WorldManager.h"


void ChunkGen(Chunk* c, WorldManager* owningWorld) {
	owningWorld->GetChunkGenerator().Gen2.GenerateChunk(c->m_Blocks, c->ChunkX, c->ChunkZ, 0);
	c->GenerateHeightMap();
	c->IsGenerated = true;
}
void ChunkPop(Chunk* c, WorldManager* owningWorld) {
	owningWorld->GetChunkGenerator().Gen2.populate(c->ChunkX, c->ChunkZ);
	c->IsModified = true;
}
void ChunkMesh(Chunk* c, WorldManager* owningWorld) {
	c->CreateChunkMeshData();
	c->IsMeshed = true;
}


void LODGen(Chunk* c, WorldManager* owningWorld) {
	uint8_t LODSize = 1 << c->LOD;
	owningWorld->GetChunkGenerator().Gen2.GenerateChunk(c->m_Blocks, c->ChunkX * LODSize, c->ChunkZ * LODSize, c->LOD);
	c->IsGenerated = true;
	c->IsModified = true;
}




ChunkProvider::ChunkProvider(WorldManager* world) : owningWorld(world), pool(15) {
}
ChunkProvider::~ChunkProvider() {
}

//the chunks public functions
Chunk* ChunkProvider::ProvideChunk(int ChunkX, int ChunkZ) {
	if (IsValidChunk(ChunkX, ChunkZ)) {
		return chunks[glm::ivec2(ChunkX, ChunkZ)];
	}
	
	//create new chunk
	Chunk* c = LoadNewChunk(ChunkX, ChunkZ, 0);
	chunks[glm::ivec2(ChunkX, ChunkZ)] = c;
	c->CreateMeshObjects();

	pool.QueueJob({ ChunkGen, c, owningWorld});

	return c;
}
void ChunkProvider::MeshChunk(Chunk* c) {
	pool.QueueJob({ ChunkMesh, c, owningWorld });
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

	bool A = IsValidChunk(X + 1, Z) && ProvideChunk(X + 1, Z)->IsPopulated;
	bool B = IsValidChunk(X - 1, Z) && ProvideChunk(X - 1, Z)->IsPopulated;
	bool C = IsValidChunk(X, Z + 1) && ProvideChunk(X, Z + 1)->IsPopulated;
	bool D = IsValidChunk(X, Z - 1) && ProvideChunk(X, Z - 1)->IsPopulated;

	return A && B && C && D;
}
std::unordered_map<glm::ivec2, Chunk*>& ChunkProvider::GetAllChunks() {
	return chunks;
}


//the LODs public functions
Chunk* ChunkProvider::ProvideLOD(int ChunkX, int ChunkZ, uint8_t LOD) {
	std::unordered_map<glm::ivec2, Chunk*>& MAP = GetLODMap(LOD);
	if (IsValidLOD(ChunkX, ChunkZ, LOD)) {
		return MAP[glm::ivec2(ChunkX, ChunkZ)];
	}

	//create new LOD
	Chunk* c = LoadNewLOD(ChunkX, ChunkZ, LOD);
	MAP[glm::ivec2(ChunkX, ChunkZ)] = c;
	c->CreateMeshObjects();

	pool.QueueJob({ LODGen, c, owningWorld });

	return c;
}
void ChunkProvider::MeshLOD(Chunk* c) {
	pool.QueueJob({ ChunkMesh, c, owningWorld });
}
std::unordered_map<glm::ivec2, Chunk*>& ChunkProvider::GetAllLODs(uint8_t LOD) {
	return GetLODMap(LOD);
}



//chunks checking
bool ChunkProvider::IsValidChunk(int ChunkX, int ChunkZ) {
	return chunks.find(glm::ivec2(ChunkX, ChunkZ)) != chunks.end(); //basically return true if this chunk is loaded (keyword: LOADED!!!)
}
Chunk* ChunkProvider::LoadNewChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	Chunk* chunk = new Chunk();
	chunk->LOD = LOD;
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningWorld = owningWorld;

	return chunk;
}
void ChunkProvider::DeleteChunkAt(int ChunkX, int ChunkZ) {
	Chunk* c = chunks[glm::ivec2(ChunkX, ChunkZ)];

	c->DeleteMeshObjects();
	delete c;
}

//LODs checking
std::unordered_map<glm::ivec2, Chunk*>& ChunkProvider::GetLODMap(uint8_t LOD) {
	return LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;
}
bool ChunkProvider::IsValidLOD(int ChunkX, int ChunkZ, uint8_t LOD) {
	std::unordered_map<glm::ivec2, Chunk*>& MAP = GetLODMap(LOD);
	return MAP.find(glm::ivec2(ChunkX, ChunkZ)) != MAP.end(); //basically returns true if this LOD is loaded (keyword: LOADED!!!)
}
Chunk* ChunkProvider::LoadNewLOD(int ChunkX, int ChunkZ, uint8_t LOD) {
	Chunk* chunk = new Chunk;
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
		pool.QueueJob({ChunkPop, c, owningWorld});
	}
}