#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave3.h"

class WorldManager;
class TerrainGen_3
{
public:
	TerrainGen_3(Random& random, WorldManager* world);
	~TerrainGen_3();

	//generates Alpha terrain
	void GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD);
private:
	WorldManager* owningWorld = nullptr;
	Random& Rand;

	//all noise maps for Alpha terrain
	NoiseGeneratorOctave3 noiseGen1;
	NoiseGeneratorOctave3 noiseGen2;
	NoiseGeneratorOctave3 noiseGen3;
	NoiseGeneratorOctave3 noiseGen4;
	NoiseGeneratorOctave3 noiseGen5;
	NoiseGeneratorOctave3 noiseGen6;
	NoiseGeneratorOctave3 noiseGen7;
	NoiseGeneratorOctave3 mobSpawnerNoise;

	double* noiseArray = nullptr;
	double* noise3 = nullptr;
	double* noise1 = nullptr;
	double* noise2 = nullptr;
	double* noise6 = nullptr;
	double* noise7 = nullptr;

	double* sandNoise = nullptr;
	double* gravelNoise = nullptr;
	double* stoneNoise = nullptr;

	void generateTerrain(int var1, int var2, BlockType* voxelData);
	void replaceSurfaceBlocks(int var1, int var2, BlockType* voxelData);

	double* initializeNoiseField(double* var1, int var2, int var3, int var4, int var5, int var6, int var7);
};

