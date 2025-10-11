#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave.h"
#include "NoiseGeneratorOctave2.h"
#include "NoiseGeneratorOctave3.h"


class WorldManager;
class ChunkGenerator
{
public:
	WorldManager* owningWorld;

	ChunkGenerator();
	~ChunkGenerator();

	void GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD); //generates infdev 2010-02-27 terrain
	void GenerateChunk2(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD); //generates infdev 2010-03-27 terrain
	void GenerateChunk3(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD); //generates infdev 2010-04-20 up to Alpha 1.1.2_01 terrain

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

	//all noise maps for infdev 2010-04-20 up to Alpha 1.1.2_01 terrain generation
	NoiseGeneratorOctave3 noiseGen1_3;
	NoiseGeneratorOctave3 noiseGen2_3;
	NoiseGeneratorOctave3 noiseGen3_3;
	NoiseGeneratorOctave3 noiseGen4_3;
	NoiseGeneratorOctave3 noiseGen5_3;
	double* noiseArray = nullptr;
	double* noise3 = nullptr;
	double* noise1 = nullptr;
	double* noise2 = nullptr;
	int noise3Len = 0;
	int noise1Len = 0;
	int noise2Len = 0;

	//helpers
	void GenerateOre(int X, int Y, int Z, BlockType type);
	void GenerateTree(int X, int Y, int Z);
};

