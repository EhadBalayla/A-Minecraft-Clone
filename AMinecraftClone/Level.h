#pragma once
#include "Chunk.h"
#include <unordered_map>
#include <vector>


constexpr int Chunks_X = 16;

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

class Level
{
public:
	unsigned int seed;


	Level();
	~Level();
	

	void RenderLevel();

	void UpdateChunks(int ChunkX, int ChunkZ); //basically called for when the player switches a chunk
	

	bool IsValidChunk(int ChunkX, int ChunkZ);

	Chunk* getChunkAt(int ChunkX, int ChunkZ);
	Block* getBlockAt(int x, int y, int z);
	AABB getBlockHitbox(int x, int y, int z);
	bool IsSolidBlock(int x, int y, int z);

	void PlaceBlock(int x, int y, int z, BlockType type); //a function for placing a block
private:

	std::unordered_map<glm::ivec2, Chunk*> chunks;

	void LoadNewChunk(int ChunkX, int ChunkZ); //loads a new chunk
	void RemoveChunk(int ChunkX, int ChunkZ);

	//all noise maps for terrain generation
	CombinedNoise* var6;
	CombinedNoise* var7;
	OctaveNoise* var8;
	OctaveNoise* var54;

	std::vector<glm::ivec2> chunkRemovalQueue;
	void DeleteRemovalQueuedChunks(); //a helper function to remove any deleted chunk from the hashmap, will be called after deleting
};

