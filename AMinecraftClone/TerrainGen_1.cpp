#include "TerrainGen_1.h"

TerrainGen_1::TerrainGen_1(Random& random) : Rand(random), 
noiseGen1(16), noiseGen2(16), noiseGen3(8), noiseGen4(4), noiseGen5(4), noiseGen6(5) {

}

void TerrainGen_1::GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD) {
    uint8_t LODFactor = 1 << LOD;

    int BaseX = ChunkX << 4;
    int BaseZ = ChunkZ << 4;

    for (int x = 0; x < Chunk_Width; x++) {
        for (int z = 0; z < Chunk_Length; z++) {
            int WorldX = x * LODFactor + BaseX;
            int WorldZ = z * LODFactor + BaseZ;

            int var7 = WorldX / 1024;
            int var8 = WorldZ / 1024;

            float var9 = (float)(noiseGen1.generateNoise(WorldX / 0.03125, 0.0, WorldZ / 0.03125) - noiseGen2.generateNoise(WorldX / 0.015625, 0.0, WorldZ / 0.015625)) / 512.0f / 4.0f;
            float var10 = (float)noiseGen5.generateNoise(WorldX / 4.0, WorldZ / 4.0);
            float var11 = (float)noiseGen6.generateNoise(WorldX / 8.0, WorldZ / 8.0) / 8.0;

            var10 = var10 > 0.0f ? (float)(noiseGen3.generateNoise(WorldX * 0.25714284 * 2.0, WorldZ * 0.25714284 * 2.0) * (double)var11 / 4.0) : (float)(noiseGen4.generateNoise((double)((float)WorldX * 0.25714284f), (double)((float)WorldZ * 0.25714284f)) * (double)var11);
            int FinalHeight = (int)(var9 + 64.0f + var10);
            if ((float)noiseGen5.generateNoise(WorldX, WorldZ) < 0.0f) {
                FinalHeight = FinalHeight / 2 << 1;
                if ((float)noiseGen5.generateNoise(WorldX / 5, WorldZ / 5) < 0.0f) {
                    ++FinalHeight;
                }
            }

            for (int y = 0; y < Chunk_Height; y++) {
                double randomDouble = rand() / (RAND_MAX + 1.0);

                if ((WorldX == 0 || WorldZ == 0) && y <= FinalHeight + 2) {
                    voxelData[IndexAt(x, y, z)] = BlockType::Obsidian; //the obsidian cross in x = 0 and z = 0
                }
                else if (LOD <= 0 && y == FinalHeight + 1 && FinalHeight >= 64 && randomDouble < 0.02) {
                    voxelData[IndexAt(x, y, z)] = BlockType::YellowFlower;
                }
                else if (y == FinalHeight && FinalHeight >= 64) {
                    voxelData[IndexAt(x, y, z)] = BlockType::Grass; //the grass layer
                }
                else if (y <= FinalHeight - 2) {
                    voxelData[IndexAt(x, y, z)] = BlockType::Stone; //the stone layers
                }
                else if (y <= FinalHeight) {
                    voxelData[IndexAt(x, y, z)] = BlockType::Dirt; //the dirt layers
                }
                else if (y <= 64) {
                    voxelData[IndexAt(x, y, z)] = BlockType::WaterStill; //places oceans
                }


                //this and on is responsible for the brick pyramid
                Rand.setSeed((long)(var7 + var8 * 13871));
                int var12 = (var7 << 10) + 128 + Rand.nextInt(512);
                int var13 = (var8 << 10) + 128 + Rand.nextInt(512);
                var12 = WorldX - var12;
                var13 = WorldZ - var13;
                if (var12 < 0) {
                    var12 = -var12;
                }

                if (var13 < 0) {
                    var13 = -var13;
                }

                if (var13 > var12) {
                    var12 = var13;
                }

                var12 = 127 - var12;
                if (var12 == 255) {
                    var12 = 1;
                }

                if (var12 < FinalHeight) {
                    var12 = FinalHeight;
                }

                if (y <= var12 && (voxelData[IndexAt(x, y, z)] == BlockType::Air || voxelData[IndexAt(x, y, z)] == BlockType::WaterStill)) {
                    voxelData[IndexAt(x, y, z)] = BlockType::Bricks;
                }
            }
        }
    }
}