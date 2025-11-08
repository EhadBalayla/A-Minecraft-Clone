#pragma once
#include "Chunk.h"
#include "TerrainGen_1.h"
#include "TerrainGen_2.h"
#include "TerrainGen_3.h"
#include "NoiseGeneratorOctave3.h"


class WorldManager;
class ChunkGenerator
{
public:
	ChunkGenerator(WorldManager* world);
	~ChunkGenerator();
	
	TerrainGen_1 Gen1;
	TerrainGen_2 Gen2;
	TerrainGen_3 Gen3;
};

