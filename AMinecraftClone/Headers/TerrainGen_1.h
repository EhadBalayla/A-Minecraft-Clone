#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave.h"
#include "NoiseGeneratorOctave2.h"

class WorldManager;
class TerrainGen_1
{
public:
	TerrainGen_1(long seed, WorldManager* world);
	WorldManager* owningWorld;

	//generates infdev 2010-02-27 terrain
	void GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD);
	void populate(int ChunkX, int ChunkZ, uint8_t LOD);
private:
	Random Rand; //reference to the random

	//all noise maps for infdev 2010-02-27 terrain generation
	NoiseGeneratorOctave noiseGen1;
	NoiseGeneratorOctave noiseGen2;
	NoiseGeneratorOctave noiseGen3;
	NoiseGeneratorOctave noiseGen4;
	NoiseGeneratorOctave noiseGen5;
	NoiseGeneratorOctave noiseGen6;
	NoiseGeneratorOctave2 mobSpawnerNoise;

	void GenerateOre(int X, int Y, int Z, BlockType type, uint8_t LOD);
};

