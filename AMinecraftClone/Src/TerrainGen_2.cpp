#include "TerrainGen_2.h"
#include "WorldManager.h"
#include "Game.h"
#include "MathHelper.h"

#define PI 3.14159265358979323846

//declarations of helper functions:
double initializeNoiseField(double var1, double var3, double var5, NoiseGeneratorOctave2& noiseGen1, NoiseGeneratorOctave2& noiseGen2, NoiseGeneratorOctave2& noiseGen3);


TerrainGen_2::TerrainGen_2(long seed, WorldManager* world) : Rand(Random(seed)), randomSeed(seed), owningWorld(world), 
noiseGen1(Rand, 16), noiseGen2(Rand, 16), noiseGen3(Rand, 8), mobSpawnerNoise(Rand, 5) {

}

void TerrainGen_2::GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD) {
    uint8_t LODFactor = 1 << LOD;

    Rand.setSeed((long)ChunkX * 341873128712L + (long)ChunkZ * 132897987541L);

    for (int cx = 0; cx < 4; ++cx) {
        for (int cz = 0; cz < 4; ++cz) {
            double noise[33][4];
            int worldX = (ChunkX << 2) + cx * LODFactor;
            int worldZ = (ChunkZ << 2) + cz * LODFactor;

            for (int y = 0; y <= 32; ++y) {
                noise[y][0] = initializeNoiseField(worldX, y, worldZ, noiseGen1, noiseGen2, noiseGen3);
                noise[y][1] = initializeNoiseField(worldX, y, worldZ + LODFactor, noiseGen1, noiseGen2, noiseGen3);
                noise[y][2] = initializeNoiseField(worldX + LODFactor, y, worldZ, noiseGen1, noiseGen2, noiseGen3);
                noise[y][3] = initializeNoiseField(worldX + LODFactor, y, worldZ + LODFactor, noiseGen1, noiseGen2, noiseGen3);
            }

            for (int y = 0; y < 32; ++y) {
                for (int dy = 0; dy < 4; ++dy) {
                    double lerpY = (double)dy / 4.0;

                    // Interpolate noise between corners
                    double n00 = noise[y][0] + (noise[y + 1][0] - noise[y][0]) * lerpY;
                    double n01 = noise[y][1] + (noise[y + 1][1] - noise[y][1]) * lerpY;
                    double n10 = noise[y][2] + (noise[y + 1][2] - noise[y][2]) * lerpY;
                    double n11 = noise[y][3] + (noise[y + 1][3] - noise[y][3]) * lerpY;

                    for (int dx = 0; dx < 4; ++dx) {
                        double lerpX = (double)dx / 4.0;
                        double nx0 = n00 + (n10 - n00) * lerpX;
                        double nx1 = n01 + (n11 - n01) * lerpX;

                        for (int dz = 0; dz < 4; ++dz) {
                            double lerpZ = (double)dz / 4.0;
                            double finalNoise = nx0 + (nx1 - nx0) * lerpZ;

                            int blockX = cx * 4 + dx;
                            int blockY = y * 4 + dy;
                            int blockZ = cz * 4 + dz;

                            if (blockX >= 16 || blockY >= 128 || blockZ >= 16)
                                continue;

                            BlockType newType = BlockType::Air;
                            if (blockY < 64)
                                newType = BlockType::WaterStill;
                            if (finalNoise > 0.0)
                                newType = BlockType::Stone;

                            voxelData[IndexAt(blockX, blockY, blockZ)] = newType;
                        }
                    }
                }
            }
        }
    }

    // Pass 2: replace topmost Stone with Grass or Dirt
    for (int x = 0; x < Chunk_Width; ++x) {
        for (int z = 0; z < Chunk_Length; ++z) {
            int dirtDepth = -1;
            for (int y = Chunk_Height - 1; y >= 0; --y) {

                BlockType type = voxelData[IndexAt(x, y, z)];
                if (type == BlockType::Air) {
                    dirtDepth = -1;
                }
                else if (type == BlockType::Stone) {
                    if (dirtDepth == -1) {
                        dirtDepth = 3;
                        voxelData[IndexAt(x, y, z)] = (y >= 63) ? BlockType::Grass : BlockType::Dirt;
                    }
                    else if (dirtDepth > 0) {
                        --dirtDepth;
                        voxelData[IndexAt(x, y, z)] = BlockType::Dirt;
                    }
                }
            }
        }
    }
}
void TerrainGen_2::populate(int var2, int var3, uint8_t LOD) {
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

void TerrainGen_2::GenerateOre(int var3, int var4, int var5, BlockType type, uint8_t LOD) {
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




double initializeNoiseField(double var1, double var3, double var5, NoiseGeneratorOctave2& noiseGen1, NoiseGeneratorOctave2& noiseGen2, NoiseGeneratorOctave2& noiseGen3) {
    double var7 = var3 * 4.0 - 64.0;
    if (var7 < 0.0) {
        var7 *= 3.0;
    }

    double var9 = noiseGen3.generateNoiseOctaves(var1 * 684.412 / 80.0, var3 * 684.412 / 400.0, var5 * 684.412 / 80.0) / 2.0;
    double var11;
    double var13;
    if (var9 < -1.0) {
        var11 = noiseGen1.generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0;
        var13 = var11 - var7;
        if (var13 < -10.0) {
            var13 = -10.0;
        }

        if (var13 > 10.0) {
            var13 = 10.0;
        }
    }
    else if (var9 > 1.0) {
        var11 = noiseGen2.generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0;
        var13 = var11 - var7;
        if (var13 < -10.0) {
            var13 = -10.0;
        }

        if (var13 > 10.0) {
            var13 = 10.0;
        }
    }
    else {
        double var15 = noiseGen1.generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0 - var7;
        double var17 = noiseGen2.generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0 - var7;
        if (var15 < -10.0) {
            var15 = -10.0;
        }

        if (var15 > 10.0) {
            var15 = 10.0;
        }

        if (var17 < -10.0) {
            var17 = -10.0;
        }

        if (var17 > 10.0) {
            var17 = 10.0;
        }

        double var19 = (var9 + 1.0) / 2.0;
        var11 = var15 + (var17 - var15) * var19;
        var13 = var11;
    }

    return var13;
}