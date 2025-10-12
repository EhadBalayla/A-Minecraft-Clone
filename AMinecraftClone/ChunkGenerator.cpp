#include "ChunkGenerator.h"
#include "WorldManager.h"
#include "MathHelper.h"



ChunkGenerator::ChunkGenerator(WorldManager* world) : Gen1(69420L), Gen2(69420L, world), Gen3(69420L, world) { //initializing the noise for infdev 2010-03-27 terrain

}
ChunkGenerator::~ChunkGenerator() {

}