#include "ChunkProvider.h"
#include "WorldManager.h"

ChunkProvider::ChunkProvider(WorldManager* world) : owningWorld(world) {

}


Chunk* ChunkProvider::ProvideChunk(int ChunkX, int ChunkZ) {
	if (IsValidChunk(ChunkX, ChunkZ)) {
		return chunks[glm::ivec2(ChunkX, ChunkZ)];
	}
	
	//create new chunk
	Chunk* c = LoadNewChunk(ChunkX, ChunkZ);
	chunks[glm::ivec2(ChunkX, ChunkZ)] = c;

	owningWorld->GetChunkGenerator().Gen3.GenerateChunk(c->m_Blocks, ChunkX, ChunkZ, 0);

	if (!c->IsPopulated && IsValidChunk(ChunkX + 1, ChunkZ + 1) && IsValidChunk(ChunkX, ChunkZ + 1) && IsValidChunk(ChunkX + 1, ChunkZ)) {
		populate(ChunkX, ChunkZ);
	}

	if (IsValidChunk(ChunkX - 1, ChunkZ) && !ProvideChunk(ChunkX - 1, ChunkZ)->IsPopulated && IsValidChunk(ChunkX - 1, ChunkZ + 1) && IsValidChunk(ChunkX, ChunkZ + 1) && IsValidChunk(ChunkX - 1, ChunkZ)) {
		populate(ChunkX - 1, ChunkZ);
	}

	if (IsValidChunk(ChunkX, ChunkZ - 1) && !ProvideChunk(ChunkX, ChunkZ - 1)->IsPopulated && IsValidChunk(ChunkX + 1, ChunkZ - 1) && IsValidChunk(ChunkX, ChunkZ - 1) && IsValidChunk(ChunkX + 1, ChunkZ)) {
		populate(ChunkX, ChunkZ - 1);
	}

	if (IsValidChunk(ChunkX - 1, ChunkZ - 1) && !ProvideChunk(ChunkX - 1, ChunkZ - 1)->IsPopulated && IsValidChunk(ChunkX - 1, ChunkZ - 1) && IsValidChunk(ChunkX, ChunkZ - 1) && IsValidChunk(ChunkX - 1, ChunkZ)) {
		populate(ChunkX - 1, ChunkZ - 1);
	}

	return c;
}
std::unordered_map<glm::ivec2, Chunk*>& ChunkProvider::GetAllChunks() {
	return chunks;
}

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

void ChunkProvider::populate(int ChunkX, int ChunkZ) {
	Chunk* c = ProvideChunk(ChunkX, ChunkZ);
	if (!c->IsPopulated) {
		c->IsPopulated = true;
		owningWorld->GetChunkGenerator().Gen3.populate(ChunkX, ChunkZ);
		c->IsModified = true;
	}
}