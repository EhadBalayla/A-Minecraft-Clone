#include "ChunkProvider.h"
#include "WorldManager.h"

#include "Utilities.h"

void ChunkGen(Chunk* c, WorldManager* owningWorld) {
	int LODFactor = GetLODSize(c->LOD);
	owningWorld->GetChunkGenerator().Gen2.GenerateChunk(c->m_Blocks, c->ChunkX * LODFactor, c->ChunkZ * LODFactor, c->LOD);
	c->GenerateHeightMap();
	c->IsGenerated = true;
	if (c->LOD > 0) c->IsModified = true;
}
void ChunkPop(Chunk* c, WorldManager* owningWorld) {
	owningWorld->GetChunkGenerator().Gen2.populate(c->ChunkX, c->ChunkZ);
	c->IsModified = true;
}
void ChunkMesh(Chunk* c, WorldManager* owningWorld) {
	c->CreateChunkMeshData();
	c->IsMeshed = true;
}



ChunkProvider::ChunkProvider(WorldManager* world) : owningWorld(world), pool(15) {
}
ChunkProvider::~ChunkProvider() {
}

//the chunks public functions
Chunk* ChunkProvider::ProvideChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	if (IsValidChunk(ChunkX, ChunkZ, LOD)) {
		return GetAllChunks(LOD)[glm::ivec2(ChunkX, ChunkZ)];
	}
	
	//create new chunk
	Chunk* c = LoadNewChunk(ChunkX, ChunkZ, LOD);
	GetAllChunks(LOD)[glm::ivec2(ChunkX, ChunkZ)] = c;
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

	if (!c->IsPopulated && IsValidChunk(ChunkX + 1, ChunkZ + 1, c->LOD) && IsValidChunk(ChunkX, ChunkZ + 1, c->LOD) && IsValidChunk(ChunkX + 1, ChunkZ, c->LOD)) {
		populate(ChunkX, ChunkZ);
	}
}
bool ChunkProvider::HasAllNeighbors(Chunk* c) {
	int X = c->ChunkX;
	int Z = c->ChunkZ;

	bool A = IsValidChunk(X + 1, Z, c->LOD) && ProvideChunk(X + 1, Z, c->LOD)->IsPopulated;
	bool B = IsValidChunk(X - 1, Z, c->LOD) && ProvideChunk(X - 1, Z, c->LOD)->IsPopulated;
	bool C = IsValidChunk(X, Z + 1, c->LOD) && ProvideChunk(X, Z + 1, c->LOD)->IsPopulated;
	bool D = IsValidChunk(X, Z - 1, c->LOD) && ProvideChunk(X, Z - 1, c->LOD)->IsPopulated;

	return A && B && C && D;
}
std::unordered_map<glm::ivec2, Chunk*>& ChunkProvider::GetAllChunks(uint8_t LOD) {
	return LOD == 0 ? LOD0 : LOD == 1 ? LOD1 : LOD == 2 ? LOD2 : LOD == 3 ? LOD3 : LOD4;
}

void ChunkProvider::QueueChunkForDeletion(Chunk* c) {
	deletionQueue.insert(c);
}
void ChunkProvider::FlushDeletionQueue() {
	for (auto it = deletionQueue.begin(); it != deletionQueue.end();) {
		Chunk* c = *it;

		if (IsChunkInRenderDistance(c)) {
			//if the chunk somehow ended up back in the render distance while looking for flushing... then remove it from the deletion queue
			it = deletionQueue.erase(it);
			continue;
		}
		else if (c->IsGenerated && c->IsPopulated && !c->IsMeshed && !c->IsModified && c->IsRenderReady) {
			GetAllChunks(c->LOD).erase(glm::ivec2(c->ChunkX, c->ChunkZ));
				
			c->DeleteMeshObjects();
			delete c;
			it = deletionQueue.erase(it);
			continue;
		}

		it++;
	}
}



//chunks checking
bool ChunkProvider::IsValidChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	std::unordered_map<glm::ivec2, Chunk*>& MAP = GetAllChunks(LOD);
	return MAP.find(glm::ivec2(ChunkX, ChunkZ)) != MAP.end(); //basically return true if this chunk is loaded (keyword: LOADED!!!)
}
Chunk* ChunkProvider::LoadNewChunk(int ChunkX, int ChunkZ, uint8_t LOD) {
	Chunk* chunk = new Chunk;
	chunk->LOD = LOD;
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningWorld = owningWorld;

	return chunk;
}

void ChunkProvider::populate(int ChunkX, int ChunkZ) {
	Chunk* c = ProvideChunk(ChunkX, ChunkZ, 0);
	if (!c->IsPopulated) {
		c->IsPopulated = true;
		pool.QueueJob({ChunkPop, c, owningWorld});
	}
}