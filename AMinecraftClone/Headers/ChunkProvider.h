#pragma once
#include "Chunk.h"

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
	SuperChunk* ProvideLOD(int ChunkX, int Chunkz, uint8_t LOD);
	void MeshLOD(SuperChunk* c);
	std::unordered_map<glm::ivec2, SuperChunk*>& GetAllLODs(uint8_t LOD);
private:
	//all the caches
	std::unordered_map<glm::ivec2, Chunk*> chunks;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD1;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD2;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD3;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD4;

	//the chunks validation functions
	bool IsValidChunk(int ChunkX, int ChunkZ);
	Chunk* LoadNewChunk(int ChunkX, int ChunkZ);
	void DeleteChunkAt(int ChunkX, int ChunkZ);

	//the LOD validation functions
	std::unordered_map<glm::ivec2, SuperChunk*>& GetLODMap(uint8_t LOD);
	bool IsValidLOD(int ChunkX, int ChunkZ, uint8_t LOD);
	SuperChunk* LoadNewLOD(int ChunkX, int ChunkZ, uint8_t LOD);

	//small helpers
	void populate(int ChunkX, int ChunkZ);


	//threads
	bool ThreadsRunning = false;

	std::mutex ChunkGenMutex;
	std::condition_variable ChunkGenCV;
	std::queue<Chunk*> ChunkGenQueue;
	std::thread ChunkGenThread;
	void ChunkGen();

	std::mutex ChunkPopMutex;
	std::condition_variable ChunkPopCV;
	std::queue<Chunk*> ChunkPopQueue;
	std::thread ChunkPopThread;
	void ChunkPop();

	std::mutex ChunkMeshMutex;
	std::condition_variable ChunkMeshCV;
	std::queue<Chunk*> ChunkMeshQueue;
	std::thread ChunkMeshThread;
	void ChunkMesh();



	std::mutex LODGenMutex;
	std::condition_variable LODGenCV;
	std::queue<SuperChunk*> LODGenQueue;
	std::thread LODGenThread;
	void LODGen();
	
	std::thread LODPopThread;
	void LODPop();

	std::mutex LODMeshMutex;
	std::condition_variable LODMeshCV;
	std::queue<SuperChunk*> LODMeshQueue;
	std::thread LODMeshThread;
	void LODMesh();
};

