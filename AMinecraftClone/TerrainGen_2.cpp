#include "TerrainGen_2.h"
#include "WorldManager.h"

//declarations of helper functions:
double initializeNoiseField(double var1, double var3, double var5, NoiseGeneratorOctave2& noiseGen1, NoiseGeneratorOctave2& noiseGen2, NoiseGeneratorOctave2& noiseGen3);


TerrainGen_2::TerrainGen_2(Random& random, WorldManager* world) : Rand(random), owningWorld(world), 
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
void TerrainGen_2::Populate(int ChunkX, int ChunkZ) {
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
    


    int treeCount = static_cast<int>(mobSpawnerNoise.noiseGenerator(baseX * 0.25, baseZ * 0.25)) << 3;

    for (int i = 0; i < treeCount; ++i) {
        int x = baseX + Rand.nextInt(16);
        int z = baseZ + Rand.nextInt(16);
        int y = owningWorld->getHeightValue(x, z);

        if (y <= 0) continue;

        GenerateTree(x, y, z);
    }
}

void TerrainGen_2::GenerateOre(int X, int Y, int Z, BlockType type) {
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
void TerrainGen_2::GenerateTree(int X, int Y, int Z) {
    // Tree height: 4–6 blocks
    int height = Rand.nextInt(3) + 4;

    // First: check if there’s space
    if (Y <= 0 || Y + height + 1 > 128) return;

    bool canGrow = true;
    for (int yy = Y; yy <= Y + 1 + height; ++yy) {
        int radius = 1;
        if (yy == Y) radius = 0; // trunk base
        if (yy >= Y + 1 + height - 2) radius = 2; // canopy top

        for (int xx = X - radius; xx <= X + radius && canGrow; ++xx) {
            for (int zz = Z - radius; zz <= Z + radius && canGrow; ++zz) {
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
    BlockType soil = owningWorld->getBlockAt(X, Y - 1, Z);
    if (!soil || (soil != BlockType::Grass && soil != BlockType::Dirt))
        return;

    // Replace soil with dirt
    soil = BlockType::Dirt;

    // Leaves (top canopy)
    for (int yy = Y - 3 + height; yy <= Y + height; ++yy) {
        int canopyHeight = yy - (Y + height);
        int radius = 2 - canopyHeight / 2; // give it a fatter top

        for (int xx = X - radius; xx <= X + radius; ++xx) {
            int dx = xx - X;
            for (int zz = Z - radius; zz <= Z + radius; ++zz) {
                int dz = zz - Z;

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
        BlockType block = owningWorld->getBlockAt(X, Y + yy, Z);
        if (block && block == BlockType::Air) {
            block = BlockType::Wood;
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