#pragma once
#include "Chunk.h"
#include <unordered_map>
#include <vector>
#include <queue>

#include "NoiseGeneratorOctave.h"
#include "NoiseGeneratorOctave2.h"




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

	void LevelUpdate(float DeltaTime);
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

	//all noise maps for terrain generation
	Random rand;
	NoiseGeneratorOctave* noiseGen1;
	NoiseGeneratorOctave* noiseGen2;
	NoiseGeneratorOctave* noiseGen3;
	NoiseGeneratorOctave* noiseGen4;
	NoiseGeneratorOctave* noiseGen5;
	NoiseGeneratorOctave* noiseGen6;

	//all noise maps for second terrain generation
	NoiseGeneratorOctave2* noiseGen1_2;
	NoiseGeneratorOctave2* noiseGen2_2;
	NoiseGeneratorOctave2* noiseGen3_2;
	NoiseGeneratorOctave2* mobSpawnerNoise_2;

	float chunkGenTimer = 0.0f;
	const float chunkGenDelay = 0.05f;
	std::queue<glm::ivec2> chunkGenQueue;
	std::vector<glm::ivec2> chunkMeshGenQueue; //so we generate chunks if they have the info of all their 
	bool IsChunkNeighboorsGood(int ChunkX, int ChunkZ); //check if a chunk's neighbors are ALL generated
	void GenerateChunkMeshes();
};

