#include "ChunkGenerator.h"
#include "WorldManager.h"
#include "MathHelper.h"

//declarations of helper functions:
double initializeNoiseField(double var1, double var3, double var5, NoiseGeneratorOctave2& noiseGen1, NoiseGeneratorOctave2& noiseGen2, NoiseGeneratorOctave2& noiseGen3);



ChunkGenerator::ChunkGenerator() : 
	noiseGen1(16), noiseGen2(16), noiseGen3(8), noiseGen4(4), noiseGen5(4), noiseGen6(5), //initializing the noise for infdev 2010-02-27 terrain
	noiseGen1_2(Rand, 16), noiseGen2_2(Rand, 16), noiseGen3_2(Rand, 8), mobSpawnerNoise_2(Rand, 5) { //initializing the noise for infdev 2010-03-27 terrain
}
ChunkGenerator::~ChunkGenerator() {

}


void ChunkGenerator::GenerateChunk(Chunk& chunk, uint8_t LOD) {
    uint8_t LODFactor = 1 << LOD;

    int BaseX = chunk.ChunkX << 4;
    int BaseZ = chunk.ChunkZ << 4;

    for (int x = 0; x < Chunk_Width; x += LODFactor) {
        for (int z = 0; z < Chunk_Length; z += LODFactor) {
            int WorldX = x + BaseX;
            int WorldZ = z + BaseZ;

            int var7 = WorldX / 1024;
            int var8 = WorldZ / 1024;

            float var9 = (float)(noiseGen1.generateNoise((double)((float)WorldX / 0.03125f), 0.0, (double)((float)WorldZ / 0.03125f)) - noiseGen2.generateNoise((double)((float)WorldX / 0.015625f), 0.0, (double)((float)WorldZ / 0.015625f))) / 512.0f / 4.0f;
            float var10 = (float)noiseGen5.generateNoise((double)((float)WorldX / 4.0f), (double)((float)WorldZ / 4.0f));
            float var11 = (float)noiseGen6.generateNoise((double)((float)WorldX / 8.0f), (double)((float)WorldZ / 8.0f)) / 8.0f;

            var10 = var10 > 0.0f ? (float)(noiseGen3.generateNoise((double)((float)WorldX * 0.25714284f * 2.0f), (double)((float)WorldZ * 0.25714284f * 2.0f)) * (double)var11 / 4.0) : (float)(noiseGen4.generateNoise((double)((float)WorldX * 0.25714284f), (double)((float)WorldZ * 0.25714284f)) * (double)var11);
            int FinalHeight = (int)(var9 + 64.0f + var10);
            if ((float)noiseGen5.generateNoise((double)WorldX, (double)WorldZ) < 0.0f) {
                FinalHeight = FinalHeight / 2 << 1;
                if ((float)noiseGen5.generateNoise((double)(WorldX / 5), (double)(WorldZ / 5)) < 0.0f) {
                    ++FinalHeight;
                }
            }


            for (int y = 0; y < Chunk_Height; y += LODFactor) {
                chunk.m_Blocks[x][y][z].owningChunk = &chunk;
                chunk.m_Blocks[x][y][z].BlockX = x;
                chunk.m_Blocks[x][y][z].BlockY = y;
                chunk.m_Blocks[x][y][z].BlockZ = z;

                double randomDouble = rand() / (RAND_MAX + 1.0);

                if ((WorldX == 0 || WorldZ == 0) && y <= FinalHeight + 2) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::Obsidian); //the obsidian cross in x = 0 and z = 0
                }
                else if (y == FinalHeight + 1 && FinalHeight >= 64 && randomDouble < 0.02) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::YellowFlower);
                }
                else if (y == FinalHeight && FinalHeight >= 64) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::Grass); //the grass layer
                }
                else if (y <= FinalHeight - 2) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::Stone); //the stone layers
                }
                else if (y <= FinalHeight) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::Dirt); //the dirt layers
                }
                else if (y <= 64) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::WaterStill); //places oceans
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

                if (y <= var12 && (chunk.m_Blocks[x][y][z].getType() == BlockType::Air || chunk.m_Blocks[x][y][z].getType() == BlockType::WaterStill)) {
                    chunk.m_Blocks[x][y][z].setType(BlockType::Bricks);
                }
            }
        }
    }
}
void ChunkGenerator::GenerateChunk2(Chunk& chunk) {
    Rand.setSeed((long)chunk.ChunkX * 341873128712L + (long)chunk.ChunkZ * 132897987541L);

    for (int cx = 0; cx < 4; ++cx) {
        for (int cz = 0; cz < 4; ++cz) {
            double noise[33][4];
            int worldX = (chunk.ChunkX << 2) + cx;
            int worldZ = (chunk.ChunkZ << 2) + cz;

            for (int y = 0; y <= 32; ++y) {
                noise[y][0] = initializeNoiseField(worldX, y, worldZ, noiseGen1_2, noiseGen2_2, noiseGen3_2);
                noise[y][1] = initializeNoiseField(worldX, y, worldZ + 1, noiseGen1_2, noiseGen2_2, noiseGen3_2);
                noise[y][2] = initializeNoiseField(worldX + 1, y, worldZ, noiseGen1_2, noiseGen2_2, noiseGen3_2);
                noise[y][3] = initializeNoiseField(worldX + 1, y, worldZ + 1, noiseGen1_2, noiseGen2_2, noiseGen3_2);
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

                            chunk.m_Blocks[blockX][blockY][blockZ].setType(newType);
                        }
                    }
                }
            }
        }
    }

    // Pass 2: replace topmost Stone with Grass or Dirt
    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            int dirtDepth = -1;
            for (int y = 127; y >= 0; --y) {
                chunk.m_Blocks[x][y][z].owningChunk = &chunk;
                chunk.m_Blocks[x][y][z].BlockX = x;
                chunk.m_Blocks[x][y][z].BlockY = y;
                chunk.m_Blocks[x][y][z].BlockZ = z;

                BlockType type = chunk.m_Blocks[x][y][z].getType();
                if (type == BlockType::Air) {
                    dirtDepth = -1;
                }
                else if (type == BlockType::Stone) {
                    if (dirtDepth == -1) {
                        dirtDepth = 3;
                        chunk.m_Blocks[x][y][z].setType((y >= 63) ? BlockType::Grass : BlockType::Dirt);
                    }
                    else if (dirtDepth > 0) {
                        --dirtDepth;
                        chunk.m_Blocks[x][y][z].setType(BlockType::Dirt);
                    }
                }
            }
        }
    }
}

void ChunkGenerator::Populate(int X, int Z) {
    Rand.setSeed((long)X * 318279123L + (long)Z * 919871212);
    int baseX = X << 4; // Chunk base X in world coords
    int baseZ = Z << 4; // Chunk base Z in world coords

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
    


    int treeCount = static_cast<int>(mobSpawnerNoise_2.noiseGenerator(baseX * 0.25, baseZ * 0.25)) << 3;

    for (int i = 0; i < treeCount; ++i) {
        int x = baseX + Rand.nextInt(16);
        int z = baseZ + Rand.nextInt(16);
        int y = owningWorld->getHeightValue(x, z);

        if (y <= 0) continue;

        GenerateTree(x, y, z);
    }
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
                        Block* block = owningWorld->getBlockAt(x, y, z);
                        if (block && block->getType() == BlockType::Stone) {
                            block->setType(type);
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
                    Block* block = owningWorld->getBlockAt(xx, yy, zz);
                    if (block && block->getType() == BlockType::Air) {
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
    Block* soil = owningWorld->getBlockAt(x, y - 1, z);
    if (!soil || (soil->getType() != BlockType::Grass && soil->getType() != BlockType::Dirt))
        return;

    // Replace soil with dirt
    soil->setType(BlockType::Dirt);

    // Leaves (top canopy)
    for (int yy = y - 3 + height; yy <= y + height; ++yy) {
        int canopyHeight = yy - (y + height);
        int radius = 2 - canopyHeight / 2; // give it a fatter top

        for (int xx = x - radius; xx <= x + radius; ++xx) {
            int dx = xx - x;
            for (int zz = z - radius; zz <= z + radius; ++zz) {
                int dz = zz - z;

                Block* block = owningWorld->getBlockAt(xx, yy, zz);
                if (!block) continue;

                if ((std::abs(dx) != radius || std::abs(dz) != radius || Rand.nextInt(2) == 0)) {
                    block->setType(BlockType::Obsidian);
                }
            }
        }
    }

    // Trunk
    for (int yy = 0; yy < height; ++yy) {
        Block* block = owningWorld->getBlockAt(x, y + yy, z);
        if (block && block->getType() == BlockType::Air) {
            block->setType(BlockType::Wood);
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