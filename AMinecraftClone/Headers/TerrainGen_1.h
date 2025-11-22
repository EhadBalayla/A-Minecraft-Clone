#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave.h"

class TerrainGen_1
{
public:
	TerrainGen_1(long seed);

	//generates infdev 2010-02-27 terrain
	void GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD);
private:
	Random Rand; //reference to the random

	//all noise maps for infdev 2010-02-27 terrain generation
	NoiseGeneratorOctave noiseGen1;
	NoiseGeneratorOctave noiseGen2;
	NoiseGeneratorOctave noiseGen3;
	NoiseGeneratorOctave noiseGen4;
	NoiseGeneratorOctave noiseGen5;
	NoiseGeneratorOctave noiseGen6;
};

