#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave2.h"

class WorldManager;
class TerrainGen_2
{
public:
	TerrainGen_2(Random& random, WorldManager* world);

	//generates infdev 2010-03-27 terrain
	void GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD);
	void Populate(int ChunkX, int ChunkZ);
private:
	WorldManager* owningWorld = nullptr;
	Random& Rand; //reference to the random

	//all noise maps for infdev 2010-03-27 terrain generation
	NoiseGeneratorOctave2 noiseGen1;
	NoiseGeneratorOctave2 noiseGen2;
	NoiseGeneratorOctave2 noiseGen3;
	NoiseGeneratorOctave2 mobSpawnerNoise;

	void GenerateOre(int X, int Y, int Z, BlockType type);
	void GenerateTree(int X, int Y, int Z);
};

