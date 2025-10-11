#include "ChunkGenerator.h"
#include "WorldManager.h"
#include "MathHelper.h"

//declarations of helper functions:
double initializeNoiseField(double var1, double var3, double var5, NoiseGeneratorOctave2& noiseGen1, NoiseGeneratorOctave2& noiseGen2, NoiseGeneratorOctave2& noiseGen3);



ChunkGenerator::ChunkGenerator() : 
	noiseGen1(16), noiseGen2(16), noiseGen3(8), noiseGen4(4), noiseGen5(4), noiseGen6(5), //initializing the noise for infdev 2010-02-27 terrain
	noiseGen1_2(Rand, 16), noiseGen2_2(Rand, 16), noiseGen3_2(Rand, 8), mobSpawnerNoise_2(Rand, 5),
    noiseGen1_3(Rand, 16), noiseGen2_3(Rand, 16), noiseGen3_3(Rand, 8), noiseGen4_3(Rand, 4), noiseGen5_3(Rand, 4) { //initializing the noise for infdev 2010-03-27 terrain
}
ChunkGenerator::~ChunkGenerator() {
    if (noiseArray) delete[] noiseArray;
    if (noise1) delete[] noise1;
    if (noise2) delete[] noise2;
    if (noise3) delete[] noise3;
}


void ChunkGenerator::GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD) {
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
void ChunkGenerator::GenerateChunk2(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD) {
    uint8_t LODFactor = 1 << LOD;

    Rand.setSeed((long)ChunkX * 341873128712L + (long)ChunkZ * 132897987541L);

    for (int cx = 0; cx < 4; ++cx) {
        for (int cz = 0; cz < 4; ++cz) {
            double noise[33][4];
            int worldX = (ChunkX << 2) + cx * LODFactor;
            int worldZ = (ChunkZ << 2) + cz * LODFactor;

            for (int y = 0; y <= 32; ++y) {
                noise[y][0] = initializeNoiseField(worldX, y, worldZ, noiseGen1_2, noiseGen2_2, noiseGen3_2);
                noise[y][1] = initializeNoiseField(worldX, y, worldZ + LODFactor, noiseGen1_2, noiseGen2_2, noiseGen3_2);
                noise[y][2] = initializeNoiseField(worldX + LODFactor, y, worldZ, noiseGen1_2, noiseGen2_2, noiseGen3_2);
                noise[y][3] = initializeNoiseField(worldX + LODFactor, y, worldZ + LODFactor, noiseGen1_2, noiseGen2_2, noiseGen3_2);
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
void ChunkGenerator::GenerateChunk3(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD) {
    Rand.setSeed((long)ChunkX * 341873128712L + (long)ChunkZ * 132897987541L);

    int var10003 = ChunkX << 2;
    int var10005 = ChunkZ << 2;
    bool var5 = true;
    var5 = true;
    var5 = true;
    int var8 = var10005;
    var5 = false;
    int var7 = var10003;
    if (noiseArray == nullptr) {
        noiseArray = new double[425] {0.0};
    }

    noise3 = noiseGen3_3.generateNoiseOctaves(&noise3, &noise3Len, var7, 0, var8, 5, 17, 5, 8.555150000000001, 4.277575000000001, 8.555150000000001);
    noise1 = noiseGen1_3.generateNoiseOctaves(&noise1, &noise1Len, var7, 0, var8, 5, 17, 5, 684.412, 684.412, 684.412);
    noise2 = noiseGen2_3.generateNoiseOctaves(&noise2, &noise2Len, var7, 0, var8, 5, 17, 5, 684.412, 684.412, 684.412);
    var7 = 0;

    for (var8 = 0; var8 < 5; ++var8) {
        for (int var9 = 0; var9 < 5; ++var9) {
            for (int var10 = 0; var10 < 17; ++var10) {
                double var63 = ((double)var10 - 8.5) * 12.0;
                if (var63 < 0.0) {
                    var63 *= 2.0;
                }

                double var65 = noise1[var7] / 512.0;
                double var67 = noise2[var7] / 512.0;
                double var69 = (noise3[var7] / 10.0 + 1.0) / 2.0;
                double var61;
                if (var69 < 0.0) {
                    var61 = var65;
                }
                else if (var69 > 1.0) {
                    var61 = var67;
                }
                else {
                    var61 = var65 + (var67 - var65) * var69;
                }

                var61 -= var63;
                noiseArray[var7] = var61;
                ++var7;
            }
        }
    }

    int var72;
    int var73;
    for (var72 = 0; var72 < 4; ++var72) {
        for (var73 = 0; var73 < 4; ++var73) {
            for (var7 = 0; var7 < 16; ++var7) {
                double var75 = noiseArray[(var72 * 5 + var73) * 17 + var7];
                double var77 = noiseArray[(var72 * 5 + var73 + 1) * 17 + var7];
                double var12 = noiseArray[((var72 + 1) * 5 + var73) * 17 + var7];
                double var14 = noiseArray[((var72 + 1) * 5 + var73 + 1) * 17 + var7];
                double var16 = noiseArray[(var72 * 5 + var73) * 17 + var7 + 1];
                double var18 = noiseArray[(var72 * 5 + var73 + 1) * 17 + var7 + 1];
                double var20 = noiseArray[((var72 + 1) * 5 + var73) * 17 + var7 + 1];
                double var22 = noiseArray[((var72 + 1) * 5 + var73 + 1) * 17 + var7 + 1];

                for (int var24 = 0; var24 < 8; ++var24) {
                    double var25 = (double)var24 / 8.0;
                    double var27 = var75 + (var16 - var75) * var25;
                    double var29 = var77 + (var18 - var77) * var25;
                    double var31 = var12 + (var20 - var12) * var25;
                    double var33 = var14 + (var22 - var14) * var25;

                    for (int var82 = 0; var82 < 4; ++var82) {
                        double var36 = (double)var82 / 4.0;
                        double var38 = var27 + (var31 - var27) * var36;
                        double var40 = var29 + (var33 - var29) * var36;
                        int var26 = var82 + (var72 << 2) << 11 | 0 + (var73 << 2) << 7 | (var7 << 3) + var24;

                        for (int var35 = 0; var35 < 4; ++var35) {
                            double var44 = (double)var35 / 4.0;
                            double var46 = var38 + (var40 - var38) * var44;
                            BlockType var83 = BlockType::Air;
                            if ((var7 << 3) + var24 < 64) {
                                var83 = BlockType::WaterStill;
                            }

                            if (var46 > 0.0) {
                                var83 = BlockType::Stone;
                            }

                            voxelData[var26] = var83;
                            var26 += 128;
                        }
                    }
                }
            }
        }
    }

    for (var72 = 0; var72 < 16; ++var72) {
        for (var73 = 0; var73 < 16; ++var73) {
            double var74 = (double)((ChunkX << 4) + var72);
            double var76 = (double)((ChunkZ << 4) + var73);
            bool var13 = noiseGen4_3.generateNoiseOctaves(var74 * (1.0 / 32.0), var76 * (1.0 / 32.0), 0.0) + Rand.nextDouble() * 0.2 > 0.0;
            bool var78 = noiseGen4_3.generateNoiseOctaves(var76 * (1.0 / 32.0), 109.0134, var74 * (1.0 / 32.0)) + Rand.nextDouble() * 0.2 > 3.0;
            int var15 = (int)(noiseGen5_3.noiseGenerator(var74 * (1.0 / 32.0) * 2.0, var76 * (1.0 / 32.0) * 2.0) / 3.0 + 3.0 + Rand.nextDouble() * 0.25);
            int var79 = var72 << 11 | var73 << 7 | 127;
            int var17 = -1;
            BlockType var80 = BlockType::Grass;
            BlockType var19 = BlockType::Dirt;

            for (int var81 = 127; var81 >= 0; --var81) {
                if (voxelData[var79] == 0) {
                    var17 = -1;
                }
                else if (voxelData[var79] == BlockType::Stone) {
                    if (var17 == -1) {
                        if (var15 <= 0) {
                            var80 = BlockType::Air;
                            var19 = BlockType::Stone;
                        }
                        else if (var81 >= 60 && var81 <= 65) {
                            var80 = BlockType::Grass;
                            var19 = BlockType::Dirt;
                            if (var78) {
                                var80 = BlockType::Air;
                            }

                            if (var78) {
                                var19 = BlockType::Bricks; //remember to change to gravel
                            }

                            if (var13) {
                                var80 = BlockType::Sand;
                            }

                            if (var13) {
                                var19 = BlockType::Sand;
                            }
                        }

                        if (var81 < 64 && var80 == 0) {
                            var80 = BlockType::WaterStill;
                        }

                        var17 = var15;
                        if (var81 >= 63) {
                            voxelData[var79] = var80;
                        }
                        else {
                            voxelData[var79] = var19;
                        }
                    }
                    else if (var17 > 0) {
                        --var17;
                        voxelData[var79] = var19;
                    }
                }

                --var79;
            }
        }
    }
}

void ChunkGenerator::Populate(int ChunkX, int ChunkZ) {
    Rand.setSeed((long)ChunkX * 318279123L + (long)ChunkZ * 919871212);
    int baseX = ChunkX << 4; // Chunk base X in world coords
    int baseZ = ChunkZ << 4; // Chunk base Z in world coords

    for (int i = 0; i < 20; ++i) { //coal ore
        int x = baseX + Rand.nextInt(16);
        int y = Rand.nextInt(128);
        int z = baseZ + Rand.nextInt(16);
        GenerateOre(x, y, z, BlockType::CoalOre);
    }

    for (int i = 0; i < 10; ++i) { //iron ore
        int x = baseX + Rand.nextInt(16);
        int y = Rand.nextInt(64);
        int z = baseZ + Rand.nextInt(16);
        GenerateOre(x, y, z, BlockType::IronOre);
    }

    if (Rand.nextInt(2) == 0) { //gold ore
        int x = baseX + Rand.nextInt(16);
        int y = Rand.nextInt(32); 
        int z = baseZ + Rand.nextInt(16);
        GenerateOre(x, y, z, BlockType::GoldOre);
    }

    if (Rand.nextInt(8) == 0) { //diamond ore
        int x = baseX + Rand.nextInt(16);
        int y = Rand.nextInt(16);
        int z = baseZ + Rand.nextInt(16);
        GenerateOre(x, y, z, BlockType::DiamondOre);
    }
    /*


    int treeCount = static_cast<int>(mobSpawnerNoise_2.noiseGenerator(baseX * 0.25, baseZ * 0.25)) << 3;

    for (int i = 0; i < treeCount; ++i) {
        int x = baseX + Rand.nextInt(16);
        int z = baseZ + Rand.nextInt(16);
        int y = owningWorld->getHeightValue(x, z);

        if (y <= 0) continue;

        GenerateTree(x, y, z);
    }*/
}





void ChunkGenerator::GenerateOre(int X, int Y, int Z, BlockType type) {
    float angle = Rand.nextFloat() * static_cast<float>(3.14159265358979323846);

    double xStart = X + 8.0 + std::sin(angle) * 2.0;
    double xEnd = X + 8.0 - std::sin(angle) * 2.0;
    double zStart = Z + 8.0 + std::cos(angle) * 2.0;
    double zEnd = Z + 8.0 - std::cos(angle) * 2.0;

    double yStart = Y + Rand.nextInt(3) - 2;
    double yEnd = Y + Rand.nextInt(3) - 2;

    for (int step = 0; step <= 16; ++step) {
        double t = step / 16.0;
        double xPos = xStart + (xEnd - xStart) * t;
        double yPos = yStart + (yEnd - yStart) * t;
        double zPos = zStart + (zEnd - zStart) * t;

        double veinRadius = Rand.nextFloat();
        double xzRadius = (std::sin(t * static_cast<float>(3.14159265358979323846)) + 1.0) * veinRadius + 1.0;
        double yRadius = (std::sin(t * static_cast<float>(3.14159265358979323846)) + 1.0) * veinRadius + 1.0;

        int xMin = static_cast<int>(xPos - xzRadius / 2.0);
        int xMax = static_cast<int>(xPos + xzRadius / 2.0);
        int yMin = static_cast<int>(yPos - yRadius / 2.0);
        int yMax = static_cast<int>(yPos + yRadius / 2.0);
        int zMin = static_cast<int>(zPos - xzRadius / 2.0);
        int zMax = static_cast<int>(zPos + xzRadius / 2.0);

        yMin = std::max(0, yMin);
        yMax = std::min(128 - 1, yMax);

        for (int x = xMin; x <= xMax; ++x) {
            for (int y = yMin; y <= yMax; ++y) {
                for (int z = zMin; z <= zMax; ++z) {
                    double dx = (x + 0.5 - xPos) / (xzRadius / 2.0);
                    double dy = (y + 0.5 - yPos) / (yRadius / 2.0);
                    double dz = (z + 0.5 - zPos) / (xzRadius / 2.0);
                    if (dx * dx + dy * dy + dz * dz < 1.0) {
                        BlockType block = owningWorld->getBlockAt(x, y, z);
                        if (block == BlockType::Stone) {
                            owningWorld->setBlockAt(x, y, z, type);
                        }
                    }
                }
            }
        }
    }
}
void ChunkGenerator::GenerateTree(int x, int y, int z) {
    // Tree height: 4–6 blocks
    int height = Rand.nextInt(3) + 4;

    // First: check if there’s space
    if (y <= 0 || y + height + 1 > 128) return;

    bool canGrow = true;
    for (int yy = y; yy <= y + 1 + height; ++yy) {
        int radius = 1;
        if (yy == y) radius = 0; // trunk base
        if (yy >= y + 1 + height - 2) radius = 2; // canopy top

        for (int xx = x - radius; xx <= x + radius && canGrow; ++xx) {
            for (int zz = z - radius; zz <= z + radius && canGrow; ++zz) {
                if (yy >= 0 && yy < 128) {
                    BlockType block = owningWorld->getBlockAt(xx, yy, zz);
                    if (block && block == BlockType::Air) {
                        canGrow = true;
                    }
                }
                else {
                    canGrow = false;
                }
            }
        }
    }
    if (!canGrow) return;

    // Soil check
    BlockType soil = owningWorld->getBlockAt(x, y - 1, z);
    if (!soil || (soil != BlockType::Grass && soil != BlockType::Dirt))
        return;

    // Replace soil with dirt
    soil = BlockType::Dirt;

    // Leaves (top canopy)
    for (int yy = y - 3 + height; yy <= y + height; ++yy) {
        int canopyHeight = yy - (y + height);
        int radius = 2 - canopyHeight / 2; // give it a fatter top

        for (int xx = x - radius; xx <= x + radius; ++xx) {
            int dx = xx - x;
            for (int zz = z - radius; zz <= z + radius; ++zz) {
                int dz = zz - z;

                BlockType block = owningWorld->getBlockAt(xx, yy, zz);
                if (!block) continue;

                if ((std::abs(dx) != radius || std::abs(dz) != radius || Rand.nextInt(2) == 0)) {
                    block = BlockType::Obsidian;
                }
            }
        }
    }

    // Trunk
    for (int yy = 0; yy < height; ++yy) {
        BlockType block = owningWorld->getBlockAt(x, y + yy, z);
        if (block && block == BlockType::Air) {
            block = BlockType::Wood;
        }
    }
}




//helper functions
//terrain generation helper functions
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