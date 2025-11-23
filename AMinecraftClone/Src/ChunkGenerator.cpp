#include "ChunkGenerator.h"
#include "WorldManager.h"
#include "MathHelper.h"

#include <ctime> // Required for time() and time_t

long getCurrentTimeAsLong() {
    time_t currentTime = time(nullptr); // Get current calendar time
    return static_cast<long>(currentTime); // Cast time_t to long
}

ChunkGenerator::ChunkGenerator(WorldManager* world) : Gen1(69420L), Gen2(getCurrentTimeAsLong(), world), Gen3(69420L, world) {

}
ChunkGenerator::~ChunkGenerator() {

}