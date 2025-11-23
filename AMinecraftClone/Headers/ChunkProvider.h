#pragma once
#include "Chunk.h"
#include "ThreadPool.h"

#include <unordered_map>
#include <unordered_set>

namespace std {
	template<>
	struct hash<glm::ivec2> {
		std::size_t operator()(const glm::ivec2& v) const {
			std::size_t h1 = std::hash<int>()(v.x);
			std::size_t h2 = std::hash<int>()(v.y);
			return h1 ^ (h2 << 1); // combine hashes
		}
	};
}

class WorldManager;
class ChunkProvider
{
public:
	ChunkProvider(WorldManager* world);
	~ChunkProvider();

	WorldManager* owningWorld;

	//chunk related publics
	Chunk* ProvideChunk(int ChunkX, int ChunkZ, uint8_t LOD);
	void MeshChunk(Chunk* c);
	void PopChunk(Chunk* c);
	bool HasAllNeighbors(Chunk* c);
	std::unordered_map<glm::ivec2, Chunk*>& GetAllChunks(uint8_t LOD);

	void QueueChunkForDeletion(Chunk* c);
	void FlushDeletionQueue();
	void DeleteAllChunks();
private:
	//all the caches
	std::unordered_map<glm::ivec2, Chunk*> LOD0;
	std::unordered_map<glm::ivec2, Chunk*> LOD1;
	std::unordered_map<glm::ivec2, Chunk*> LOD2;
	std::unordered_map<glm::ivec2, Chunk*> LOD3;
	std::unordered_map<glm::ivec2, Chunk*> LOD4;

	std::unordered_set<Chunk*> deletionQueue;

	//the chunks validation functions
	bool IsValidChunk(int ChunkX, int ChunkZ, uint8_t LOD);
	Chunk* LoadNewChunk(int ChunkX, int ChunkZ, uint8_t LOD);

	//threads
	ThreadPool pool;
};

