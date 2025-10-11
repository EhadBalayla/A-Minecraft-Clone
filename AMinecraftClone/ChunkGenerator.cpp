#include "ChunkGenerator.h"
#include "WorldManager.h"
#include "MathHelper.h"



ChunkGenerator::ChunkGenerator(WorldManager* world) : Gen1(Rand), Gen2(Rand, world), Gen3(Rand, world) { //initializing the noise for infdev 2010-03-27 terrain

}
ChunkGenerator::~ChunkGenerator() {

}