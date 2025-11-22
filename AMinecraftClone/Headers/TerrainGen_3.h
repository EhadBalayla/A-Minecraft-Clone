#pragma once
#include "Chunk.h"
#include "NoiseGeneratorOctave3.h"

class WorldManager;
class TerrainGen_3
{
public:
	TerrainGen_3(long seed, WorldManager* world);
	~TerrainGen_3();

	//generates Alpha terrain
	void GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD);
	void populate(int ChunkX, int ChunkZ);
private:
	WorldManager* owningWorld = nullptr;
	long randomSeed;
	Random Rand;

	//specifically for the cave generation
	Random CavesRand;
	int cavesRange = 8;


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

	//terrain gen
	void generateTerrain(int var1, int var2, BlockType* voxelData);
	void replaceSurfaceBlocks(int var1, int var2, BlockType* voxelData);
	void GenerateCaves(int var3, int var4, BlockType* var5);

	//population gen
	void GenerateOre(int numberOfBlocks, BlockType type, Random& var2, int var3, int var4, int var5);
	void GenerateClay(int numberOfBlocks, Random& var2, int var3, int var4, int var5);
	void GenerateDungeon(Random& var2, int var3, int var4, int var5);
	void GenerateFlowers(BlockType flowerType, Random& var2, int var3, int var4, int var5);
	void GenerateReed(Random& var2, int var3, int var4, int var5);
	void GenerateCactus(Random& var2, int var3, int var4, int var5);
	void GenerateLiquids(BlockType type, Random& var2, int var3, int var4, int var5);
	void GenerateTree(Random& var2, int var3, int var4, int var5);
	void GenerateBigTree(Random& var2, int var3, int var4, int var5);
	
	//generation AND population helpers
	double* initializeNoiseField(double* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	void recursiveGenerate(int var2, int var3, int var4, int var5, BlockType* var6);
	void generateLargeCaveNode(int var1, int var2, BlockType* var3, double var4, double var6, double var8);
	void generateCaveNode(int var1, int var2, BlockType* var3, double var4, double var6, double var8, float var10, float var11, float var12, int var13, int var14, double var15);
};

