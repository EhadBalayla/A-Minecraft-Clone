#pragma once
#include <unordered_map>
#include <queue>
#include <vector>

#include "ChunkGenerator.h"

#include <thread>
#include <mutex>
#include <condition_variable>


struct SuperChunkStart {
	glm::ivec2 offset;
	glm::ivec2 center; //since we can't access the center in other threads
	uint8_t LOD;

	SuperChunk* updatedChunk; //for when we wanna UPDATE an existing superchunk, otherwise it will be nullptr
};
struct SuperChunkPrep {
	Chunk** chunks;
	size_t chunksCount;
	glm::ivec2 offset;
	uint8_t LOD;

	SuperChunk* updatedChunk; //for when we wanna UPDATE an existing superchunk, otherwise it will be nullptr
};
struct SuperChunkReady {
	SuperChunk* chunk;
	SuperChunkMeshUpload meshData;
	bool Update;
};


struct ChunkReady {
	Chunk* chunkPos;
	ChunkMeshUpload meshData;
};

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

class Level;
class WorldManager
{
public:
	Level* owningLevel;

	WorldManager();
	~WorldManager();

	void ChunksStart(int CenterX, int CenterZ); //performs world startup operations for chunks
	void UpdateChunks(int CenterX, int CenterZ); //basically called for when the player switches a chunk
	void UpdateLODs(int CenterX, int CenterZ, uint8_t LOD);
	void WorldUpdate(float DeltaTime);
	void RenderWorld();

	//chunk helpers
	bool IsValidChunk(int ChunkX, int ChunkZ);
	Chunk* getChunkAt(int ChunkX, int ChunkZ);

	//block helpers
	Block* getBlockAt(int x, int y, int z);
	Block* getBlockAtSafe(int x, int y, int z);
	void setBlockAt(int x, int y, int z, BlockType type);
	bool IsSolidBlock(int x, int y, int z);
	void PlaceBlock(int x, int y, int z, BlockType type);
	
	//other helpers
	int getHeightValue(int x, int z);
	AABB getBlockHitbox(int x, int y, int z);

	//coords converters
private:
	ChunkGenerator chunkGenerator; //the world generator itself

	//world helper functions
	void StartupLOD(int CenterX, int CenterZ, uint8_t LOD);

	Chunk* LoadNewChunk(int ChunkX, int ChunkZ, uint8_t LOD); //creates a new chunk
	SuperChunkPrep PrepSuperChunk(int ChunkX, int ChunkZ, uint8_t LOD); //creates temporary chunks to prep a super chunk
	bool IsChunkNeighboorsGood(int ChunkX, int ChunkZ);

	//the multithreading stuff
	bool running;
	std::thread chunkThread;
	std::thread chunkMeshesThread;
	std::mutex chunkMutex;
	std::mutex meshMutex;
	std::mutex finalMutex;
	std::condition_variable cv;
	std::condition_variable meshCV;

	std::thread superChunkThread;
	std::thread superChunkMeshesThread;
	std::mutex superChunkMutex;
	std::mutex superMeshMutex;
	std::mutex superFinalMutex;
	std::condition_variable superCV;
	std::condition_variable superMeshCV;

	std::unordered_map<glm::ivec2, Chunk*> chunks;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD1;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD2;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD3;
	std::unordered_map<glm::ivec2, SuperChunk*> LOD4;

	//chunks thread queues
	std::queue<glm::ivec2> chunkGenQueue;
	std::queue<Chunk*> chunkMeshGenQueue;
	std::queue<ChunkReady> chunkMeshFinalQueue;

	//super chunks thread queues
	std::queue<SuperChunkStart> superChunkGenQueue;
	std::queue<SuperChunkPrep> superChunkMeshGenQueue;
	std::queue<SuperChunkReady> superChunkMeshFinalQueue;

	//threads functions
	void ChunkThreadLoop();
	void ChunkMeshesThreadLoop();
	void LODThreadLoop();
	void LODMeshThreadLoop();
};
