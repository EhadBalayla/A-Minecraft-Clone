#pragma once
#include "Chunk.h"
#include "ThreadPool.h"

#include <unordered_map>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

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
	Chunk* ProvideChunk(int ChunkX, int ChunkZ);
	void MeshChunk(Chunk* c);
	void PopChunk(Chunk* c);
	bool HasAllNeighbors(Chunk* c);
	std::unordered_map<glm::ivec2, Chunk*>& GetAllChunks();

	//LOD related publics
	Chunk* ProvideLOD(int ChunkX, int Chunkz, uint8_t LOD);
	void MeshLOD(Chunk* c);
	std::unordered_map<glm::ivec2, Chunk*>& GetAllLODs(uint8_t LOD);
private:
	//all the caches
	std::unordered_map<glm::ivec2, Chunk*> chunks;
	std::unordered_map<glm::ivec2, Chunk*> LOD1;
	std::unordered_map<glm::ivec2, Chunk*> LOD2;
	std::unordered_map<glm::ivec2, Chunk*> LOD3;
	std::unordered_map<glm::ivec2, Chunk*> LOD4;

	//the chunks validation functions
	bool IsValidChunk(int ChunkX, int ChunkZ);
	Chunk* LoadNewChunk(int ChunkX, int ChunkZ, uint8_t LOD);
	void DeleteChunkAt(int ChunkX, int ChunkZ);

	//the LOD validation functions
	std::unordered_map<glm::ivec2, Chunk*>& GetLODMap(uint8_t LOD);
	bool IsValidLOD(int ChunkX, int ChunkZ, uint8_t LOD);
	Chunk* LoadNewLOD(int ChunkX, int ChunkZ, uint8_t LOD);

	//small helpers
	void populate(int ChunkX, int ChunkZ);

	//threads
	ThreadPool pool;
};

