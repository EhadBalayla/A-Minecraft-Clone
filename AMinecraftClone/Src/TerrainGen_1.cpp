#include "TerrainGen_1.h"

#include "Game.h"
#include "MathHelper.h"

#define PI 3.14159265358979323846



TerrainGen_1::TerrainGen_1(long seed, WorldManager* world) : Rand(Random(seed)), owningWorld(world),
noiseGen1(16), noiseGen2(16), noiseGen3(8), noiseGen4(4), noiseGen5(4), noiseGen6(5), mobSpawnerNoise(Rand, 5) {

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
void TerrainGen_1::populate(int var2, int var3, uint8_t LOD) {
    if (LOD != 0) return;

    Rand.setSeed((long)var2 * 318279123L + (long)var3 * 919871212L);
    int var18 = var2 << 4;
    var2 = var3 << 4;

    int var4;
    int var5;
    int var6;
    for (var3 = 0; var3 < 20; ++var3) {
        var4 = var18 + Rand.nextInt(16);
        var5 = Rand.nextInt(128);
        var6 = var2 + Rand.nextInt(16);
        GenerateOre(var4, var5, var6, BlockType::CoalOre, LOD);
    }

    for (var3 = 0; var3 < 10; ++var3) {
        var4 = var18 + Rand.nextInt(16);
        var5 = Rand.nextInt(64);
        var6 = var2 + Rand.nextInt(16);
        GenerateOre(var4, var5, var6, BlockType::IronOre, LOD);
    }

    if (Rand.nextInt(2) == 0) {
        var3 = var18 + Rand.nextInt(16);
        var4 = Rand.nextInt(32);
        var5 = var2 + Rand.nextInt(16);
        GenerateOre(var4, var5, var6, BlockType::GoldOre, LOD);
    }

    if (Rand.nextInt(8) == 0) {
        var3 = var18 + Rand.nextInt(16);
        var4 = Rand.nextInt(16);
        var5 = var2 + Rand.nextInt(16);
        GenerateOre(var4, var5, var6, BlockType::DiamondOre, LOD);
    }

    var3 = (int)mobSpawnerNoise.noiseGenerator((double)var18 * 0.25, (double)var2 * 0.25) << 3;

    for (var4 = 0; var4 < var3; ++var4) {
        var5 = var18 + Rand.nextInt(16);
        var6 = var2 + Rand.nextInt(16);
        int var8 = owningWorld->getHeightValue(var5, var6);
        int var7 = var5 + 2;
        int var9 = var6 + 2;
        int var10 = Rand.nextInt(3) + 4;
        bool var11 = true;
        bool var23;
        if (var8 > 0 && var8 + var10 + 1 <= 128) {
            int var12;
            int var14;
            int var15;
            int var16;
            for (var12 = var8; var12 <= var8 + 1 + var10; ++var12) {
                int var13 = 1;
                if (var12 == var8) {
                    var13 = 0;
                }

                if (var12 >= var8 + 1 + var10 - 2) {
                    var13 = 2;
                }

                for (var14 = var7 - var13; var14 <= var7 + var13 && var11; ++var14) {
                    for (var15 = var9 - var13; var15 <= var9 + var13 && var11; ++var15) {
                        if (var12 >= 0 && var12 < 128) {
                            var16 = owningWorld->getBlockAtLOD(var14, var12, var15, LOD);
                            if (var16 != BlockType::Air) {
                                var11 = false;
                            }
                        }
                        else {
                            var11 = false;
                        }
                    }
                }
            }

            if (!var11) {
                var23 = false;
            }
            else {
                var12 = owningWorld->getBlockAtLOD(var7, var8 - 1, var9, LOD);
                if ((var12 == BlockType::Grass || var12 == BlockType::Dirt) && var8 < 128 - var10 - 1) {
                    owningWorld->setBlockAtLOD(var7, var8 - 1, var9, BlockType::Dirt, LOD);

                    int var22;
                    for (var22 = var8 - 3 + var10; var22 <= var8 + var10; ++var22) {
                        var14 = var22 - (var8 + var10);
                        var15 = 1 - var14 / 2;

                        for (var16 = var7 - var15; var16 <= var7 + var15; ++var16) {
                            int var21 = var16 - var7;

                            for (var12 = var9 - var15; var12 <= var9 + var15; ++var12) {
                                int var17 = var12 - var9;
                                if ((std::abs(var21) != var15 || std::abs(var17) != var15 || Rand.nextInt(2) != 0 && var14 != 0) && Game::e_BlockRegistery[owningWorld->getBlockAtLOD(var16, var22, var12, LOD)].visibility == BlockVisiblity::Opaque) {
                                    owningWorld->setBlockAtLOD(var16, var22, var12, BlockType::Leaves, LOD);
                                }
                            }
                        }
                    }

                    for (var22 = 0; var22 < var10; ++var22) {
                        if (!Game::e_BlockRegistery[owningWorld->getBlockAtLOD(var7, var8 + var22, var9, LOD)].visibility != BlockVisiblity::Opaque) {
                            owningWorld->setBlockAtLOD(var7, var8 + var22, var9, BlockType::Wood, LOD);
                        }
                    }

                    var23 = true;
                }
                else {
                    var23 = false;
                }
            }
        }
        else {
            var23 = false;
        }
    }
}
void TerrainGen_1::GenerateOre(int var3, int var4, int var5, BlockType type, uint8_t LOD) {
    float var6 = Rand.nextFloat() * (float)PI;
    double var7 = (double)((float)(var3 + 8) + MathHelper::sin(var6) * 2.0F);
    double var9 = (double)((float)(var3 + 8) - MathHelper::sin(var6) * 2.0F);
    double var11 = (double)((float)(var5 + 8) + MathHelper::cos(var6) * 2.0F);
    double var13 = (double)((float)(var5 + 8) - MathHelper::cos(var6) * 2.0F);
    double var15 = (double)(var4 + Rand.nextInt(3) + 2);
    double var17 = (double)(var4 + Rand.nextInt(3) + 2);

    for (var3 = 0; var3 <= 16; ++var3) {
        double var20 = var7 + (var9 - var7) * (double)var3 / 16.0;
        double var22 = var15 + (var17 - var15) * (double)var3 / 16.0;
        double var24 = var11 + (var13 - var11) * (double)var3 / 16.0;
        double var26 = Rand.nextDouble();
        double var28 = (double)(MathHelper::sin((float)var3 / 16.0F * (float)PI) + 1.0F) * var26 + 1.0;
        double var30 = (double)(MathHelper::sin((float)var3 / 16.0F * (float)PI) + 1.0F) * var26 + 1.0;

        for (var4 = (int)(var20 - var28 / 2.0); var4 <= (int)(var20 + var28 / 2.0); ++var4) {
            for (var5 = (int)(var22 - var30 / 2.0); var5 <= (int)(var22 + var30 / 2.0); ++var5) {
                for (int var41 = (int)(var24 - var28 / 2.0); var41 <= (int)(var24 + var28 / 2.0); ++var41) {
                    double var35 = ((double)var4 + 0.5 - var20) / (var28 / 2.0);
                    double var37 = ((double)var5 + 0.5 - var22) / (var30 / 2.0);
                    double var39 = ((double)var41 + 0.5 - var24) / (var28 / 2.0);
                    if (var35 * var35 + var37 * var37 + var39 * var39 < 1.0 && owningWorld->getBlockAtLOD(var4, var5, var41, LOD) == BlockType::Stone) {
                        owningWorld->setBlockAtLOD(var4, var5, var41, type, LOD);
                    }
                }
            }
        }
    }
}