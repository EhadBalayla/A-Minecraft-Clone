#pragma once
#include <unordered_map>
#include <queue>
#include <vector>

#include "ChunkGenerator.h"


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

	void UpdateChunks(int ChunkX, int ChunkZ); //basically called for when the player switches a chunk
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
	std::unordered_map<glm::ivec2, Chunk*> chunks;
	ChunkGenerator chunkGenerator;

	float chunkGenTimer = 0.0f;
	const float chunkGenDelay = 0.05f;
	std::queue<glm::ivec2> chunkGenQueue;
	std::vector<glm::ivec2> chunkMeshGenQueue;

	void LoadNewChunk(int ChunkX, int ChunkZ);
	void GenerateChunkMeshes(); 
	bool IsChunkNeighboorsGood(int ChunkX, int ChunkZ); 
};

