#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave.h"
#include "NoiseGeneratorOctave2.h"


class WorldManager;
class ChunkGenerator
{
public:
	WorldManager* owningWorld;

	ChunkGenerator();
	~ChunkGenerator();

	void GenerateChunk(Block* voxelData, int ChunkX, int ChunkZ, uint8_t LOD); //generates infdev 2010-02-27 terrain
	void GenerateChunk2(Chunk& chunk); //generates infdev 2010-03-27 terrain

	void Populate(int X, int Z);
private:

	//all noise maps for infdev 2010-02-27 terrain generation
	Random Rand;
	NoiseGeneratorOctave noiseGen1;
	NoiseGeneratorOctave noiseGen2;
	NoiseGeneratorOctave noiseGen3;
	NoiseGeneratorOctave noiseGen4;
	NoiseGeneratorOctave noiseGen5;
	NoiseGeneratorOctave noiseGen6;

	//all noise maps for infdev 2010-03-27 terrain generation
	NoiseGeneratorOctave2 noiseGen1_2;
	NoiseGeneratorOctave2 noiseGen2_2;
	NoiseGeneratorOctave2 noiseGen3_2;
	NoiseGeneratorOctave2 mobSpawnerNoise_2;

	//helpers
	void GenerateOre(int X, int Y, int Z, BlockType type);
	void GenerateTree(int X, int Y, int Z);
};

