#include "Chunk.h"
#include <glad/glad.h>
#include "Level.h"
#include "MathHelper.h"
#include "Game.h"

Chunk::Chunk() {
    //generate buffer objects for opaque
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    //generate buffer objects for plants
    glGenVertexArrays(1, &m_VAO2);
    glGenBuffers(1, &m_VBO2);
    glGenBuffers(1, &m_EBO2);

    //generate buffer objects for liquids
    glGenVertexArrays(1, &m_VAO3);
    glGenBuffers(1, &m_VBO3);
    glGenBuffers(1, &m_EBO3);
}
Chunk::~Chunk() {
    //delete buffer objects for opaque
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    //delete buffer objects for plants
    glDeleteVertexArrays(1, &m_VAO2);
    glDeleteBuffers(1, &m_VBO2);
    glDeleteBuffers(1, &m_EBO2);

    //delete buffer objects for liquids
    glDeleteVertexArrays(1, &m_VAO3);
    glDeleteBuffers(1, &m_VBO3);
    glDeleteBuffers(1, &m_EBO3);
}

int Chunk::DistanceFromChunk(Chunk* chunk) { //returns the distance from a given chunk, only on a single axis tho, this is generally used for render distance
    int DistanceX = std::abs(std::abs(chunk->ChunkX) - std::abs(ChunkX));
    int DistanceZ = std::abs(std::abs(chunk->ChunkZ) - std::abs(ChunkZ));
    if (DistanceX > DistanceZ)
        return DistanceX;
    return DistanceZ;
}

// generation helper functions declarations
double initializeNoiseField(double var1, double var3, double var5, NoiseGeneratorOctave2* noiseGen1, NoiseGeneratorOctave2* noiseGen2, NoiseGeneratorOctave2* noiseGen3);
void generateOre2(Chunk* var1, Random& var2, int var3, int var4, int var5, BlockType newBlock);
void populate2(Chunk* var1, int var2, int var3, Random& rand, NoiseGeneratorOctave2* mobSpawnerNoise);
//terrain generation.
void Chunk::GenerateChunk(NoiseGeneratorOctave* NoiseGen1, NoiseGeneratorOctave* NoiseGen2, NoiseGeneratorOctave* NoiseGen3,
    NoiseGeneratorOctave* NoiseGen4, NoiseGeneratorOctave* NoiseGen5, NoiseGeneratorOctave* NoiseGen6,
    Random& Rand) {
    int BaseX = ChunkX << 4;
    int BaseZ = ChunkZ << 4;

    for (int x = 0; x < Chunk_Width; x++) {
        for (int z = 0; z < Chunk_Length; z++) {
            int WorldX = x + BaseX;
            int WorldZ = z + BaseZ;

            int var7 = WorldX / 1024;
            int var8 = WorldZ / 1024;

            float var9 = (float)(NoiseGen1->generateNoise((double)((float)WorldX / 0.03125f), 0.0, (double)((float)WorldZ / 0.03125f)) - NoiseGen2->generateNoise((double)((float)WorldX / 0.015625f), 0.0, (double)((float)WorldZ / 0.015625f))) / 512.0f / 4.0f;
            float var10 = (float)NoiseGen5->generateNoise((double)((float)WorldX / 4.0f), (double)((float)WorldZ / 4.0f));
            float var11 = (float)NoiseGen6->generateNoise((double)((float)WorldX / 8.0f), (double)((float)WorldZ / 8.0f)) / 8.0f;

            var10 = var10 > 0.0f ? (float)(NoiseGen3->generateNoise((double)((float)WorldX * 0.25714284f * 2.0f), (double)((float)WorldZ * 0.25714284f * 2.0f)) * (double)var11 / 4.0) : (float)(NoiseGen4->generateNoise((double)((float)WorldX * 0.25714284f), (double)((float)WorldZ * 0.25714284f)) * (double)var11);
            int FinalHeight = (int)(var9 + 64.0f + var10);
            if ((float)NoiseGen5->generateNoise((double)WorldX, (double)WorldZ) < 0.0f) {
                FinalHeight = FinalHeight / 2 << 1;
                if ((float)NoiseGen5->generateNoise((double)(WorldX / 5), (double)(WorldZ / 5)) < 0.0f) {
                    ++FinalHeight;
                }
            }


            for (int y = 0; y < Chunk_Height; y++) {
                m_Blocks[x][y][z].owningChunk = this;
                m_Blocks[x][y][z].BlockX = x;
                m_Blocks[x][y][z].BlockY = y;
                m_Blocks[x][y][z].BlockZ = z;

                double randomDouble = rand() / (RAND_MAX + 1.0);

                if ((WorldX == 0 || WorldZ == 0) && y <= FinalHeight + 2) {
                    m_Blocks[x][y][z].setType(BlockType::Obsidian); //the obsidian cross in x = 0 and z = 0
                }
                else if (y == FinalHeight + 1 && FinalHeight >= 64 && randomDouble < 0.02) {
                    m_Blocks[x][y][z].setType(BlockType::YellowFlower);
                }
                else if (y == FinalHeight && FinalHeight >= 64) {
                    m_Blocks[x][y][z].setType(BlockType::Grass); //the grass layer
                }
                else if (y <= FinalHeight - 2) {
                    m_Blocks[x][y][z].setType(BlockType::Stone); //the stone layers
                }
                else if (y <= FinalHeight) {
                    m_Blocks[x][y][z].setType(BlockType::Dirt); //the dirt layers
                }
                else if (y <= 64) {
                    m_Blocks[x][y][z].setType(BlockType::WaterStill); //places oceans
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

                if (y <= var12 && (m_Blocks[x][y][z].getType() == BlockType::Air || m_Blocks[x][y][z].getType() == BlockType::WaterStill)) {
                    m_Blocks[x][y][z].setType(BlockType::Bricks);
                }
            }
        }
    }
}
void Chunk::GenerateChunk2(NoiseGeneratorOctave2* noiseGen1, NoiseGeneratorOctave2* noiseGen2, NoiseGeneratorOctave2* noiseGen3, Random& Rand, NoiseGeneratorOctave2* mobSpawnerNoise) {
    Rand.setSeed((long)ChunkX * 341873128712L + (long)ChunkZ * 132897987541L);

    for (int cx = 0; cx < 4; ++cx) {
        for (int cz = 0; cz < 4; ++cz) {
            double noise[33][4];
            int worldX = (ChunkX << 2) + cx;
            int worldZ = (ChunkZ << 2) + cz;

            for (int y = 0; y <= 32; ++y) {
                noise[y][0] = initializeNoiseField(worldX, y, worldZ, noiseGen1, noiseGen2, noiseGen3);
                noise[y][1] = initializeNoiseField(worldX, y, worldZ + 1, noiseGen1, noiseGen2, noiseGen3);
                noise[y][2] = initializeNoiseField(worldX + 1, y, worldZ, noiseGen1, noiseGen2, noiseGen3);
                noise[y][3] = initializeNoiseField(worldX + 1, y, worldZ + 1, noiseGen1, noiseGen2, noiseGen3);
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

                            m_Blocks[blockX][blockY][blockZ].setType(newType);
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
                m_Blocks[x][y][z].owningChunk = this;
                m_Blocks[x][y][z].BlockX = x;
                m_Blocks[x][y][z].BlockY = y;
                m_Blocks[x][y][z].BlockZ = z;

                BlockType type = m_Blocks[x][y][z].getType();
                if (type == BlockType::Air) {
                    dirtDepth = -1;
                }
                else if (type == BlockType::Stone) {
                    if (dirtDepth == -1) {
                        dirtDepth = 3;
                        m_Blocks[x][y][z].setType((y >= 63) ? BlockType::Grass : BlockType::Dirt);
                    }
                    else if (dirtDepth > 0) {
                        --dirtDepth;
                        m_Blocks[x][y][z].setType(BlockType::Dirt);
                    }
                }
            }
        }
    }

    //Pass 3: decorate world
    //populate2(this, ChunkX, ChunkZ, Rand, mobSpawnerNoise);
}

//terrain generation helper functions
double initializeNoiseField(double var1, double var3, double var5,
    NoiseGeneratorOctave2* noiseGen1, NoiseGeneratorOctave2* noiseGen2, NoiseGeneratorOctave2* noiseGen3) {
    double var7 = var3 * 4.0 - 64.0;
    if (var7 < 0.0) {
        var7 *= 3.0;
    }

    double var9 = noiseGen3->generateNoiseOctaves(var1 * 684.412 / 80.0, var3 * 684.412 / 400.0, var5 * 684.412 / 80.0) / 2.0;
    double var11;
    double var13;
    if (var9 < -1.0) {
        var11 = noiseGen1->generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0;
        var13 = var11 - var7;
        if (var13 < -10.0) {
            var13 = -10.0;
        }

        if (var13 > 10.0) {
            var13 = 10.0;
        }
    }
    else if (var9 > 1.0) {
        var11 = noiseGen2->generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0;
        var13 = var11 - var7;
        if (var13 < -10.0) {
            var13 = -10.0;
        }

        if (var13 > 10.0) {
            var13 = 10.0;
        }
    }
    else {
        double var15 = noiseGen1->generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0 - var7;
        double var17 = noiseGen2->generateNoiseOctaves(var1 * 684.412, var3 * 984.412, var5 * 684.412) / 512.0 - var7;
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
void generateOre2(Chunk* var1, Random& var2, int var3, int var4, int var5, BlockType newBlock) {
    float var6 = var2.nextFloat() * 3.14159265358979323846f;
    double var7 = (double)((float)(var3 + 8) + MathHelper::sin(var6) * 2.0f);
    double var9 = (double)((float)(var3 + 8) - MathHelper::sin(var6) * 2.0f);
    double var11 = (double)((float)(var5 + 8) + MathHelper::cos(var6) * 2.0f);
    double var13 = (double)((float)(var5 + 8) - MathHelper::cos(var6) * 2.0f);
    double var15 = (double)(var4 + var2.nextInt(3) + 2);
    double var17 = (double)(var4 + var2.nextInt(3) + 2);

    for (var3 = 0; var3 <= 16; ++var3) {
        double var20 = var7 + (var9 - var7) * (double)var3 / 16.0;
        double var22 = var15 + (var17 - var15) * (double)var3 / 16.0;
        double var24 = var11 + (var13 - var11) * (double)var3 / 16.0;
        double var26 = var2.nextDouble();
        double var28 = (double)(MathHelper::sin((float)var3 / 16.0f * 3.14159265358979323846f) + 1.0f) * var26 + 1.0;
        double var30 = (double)(MathHelper::sin((float)var3 / 16.0f * 3.14159265358979323846f) + 1.0f) * var26 + 1.0;

        for (var4 = (int)(var20 - var28 / 2.0); var4 <= (int)(var20 + var28 / 2.0); ++var4) {
            for (var5 = (int)(var22 - var30 / 2.0); var5 <= (int)(var22 + var30 / 2.0); ++var5) {
                for (int var41 = (int)(var24 - var28 / 2.0); var41 <= (int)(var24 + var28 / 2.0); ++var41) {
                    double var35 = ((double)var4 + 0.5 - var20) / (var28 / 2.0);
                    double var37 = ((double)var5 + 0.5 - var22) / (var30 / 2.0);
                    double var39 = ((double)var41 + 0.5 - var24) / (var28 / 2.0);
                    if (var35 * var35 + var37 * var37 + var39 * var39 < 1.0 && var1->m_Blocks[var4][var5][var41].getType() == BlockType::Stone) {
                        var1->m_Blocks[var4][var5][var41].setType(newBlock);
                    }
                }
            }
        }
    }
}
void populate2(Chunk* var1, int var2, int var3, Random& rand, NoiseGeneratorOctave2* mobSpawnerNoise) {
    rand.setSeed((long)var2 * 318279123L + (long)var3 * 919871212L);
    int var18 = var2 << 4;
    var2 = var3 << 4;

    int var4;
    int var5;
    int var6;
    for (var3 = 0; var3 < 20; ++var3) {
        var4 = var18 + rand.nextInt(16);
        var5 = rand.nextInt(128);
        var6 = var2 + rand.nextInt(16);
        generateOre2(var1, rand, var4, var5, var6, BlockType::CoalOre);
    }

    for (var3 = 0; var3 < 10; ++var3) {
        var4 = var18 + rand.nextInt(16);
        var5 = rand.nextInt(64);
        var6 = var2 + rand.nextInt(16);
        generateOre2(var1, rand, var4, var5, var6, BlockType::IronOre);
    }

    if (rand.nextInt(2) == 0) {
        var3 = var18 + rand.nextInt(16);
        var4 = rand.nextInt(32);
        var5 = var2 + rand.nextInt(16);
        generateOre2(var1, rand, var4, var5, var6, BlockType::GoldOre);
    }

    if (rand.nextInt(8) == 0) {
        var3 = var18 + rand.nextInt(16);
        var4 = rand.nextInt(16);
        var5 = var2 + rand.nextInt(16);
        generateOre2(var1, rand, var4, var5, var6, BlockType::DiamondOre);
    }

    var3 = (int)mobSpawnerNoise->noiseGenerator((double)var18 * 0.25, (double)var2 * 0.25) << 3;

    for (var4 = 0; var4 < var3; ++var4) {
        var5 = var18 + rand.nextInt(16);
        var6 = var2 + rand.nextInt(16);
        Random& var10001 = rand;
        int var8 = 100;
        Random& var20 = var10001;
        int var7 = var5 + 2;
        int var9 = var6 + 2;
        int var10 = var20.nextInt(3) + 4;
        bool var11 = true;
        bool var23;
        if (var8 > 0 && var8 + var10 + 1 <= 128) {
            BlockType var12;
            int var14;
            int var15;
            BlockType var16;
            for (int p = var8; p <= var8 + 1 + var10; ++p) {
                int var13 = 1;
                if (p == var8) {
                    var13 = 0;
                }

                if (p >= var8 + 1 + var10 - 2) {
                    var13 = 2;
                }

                for (var14 = var7 - var13; var14 <= var7 + var13 && var11; ++var14) {
                    for (var15 = var9 - var13; var15 <= var9 + var13 && var11; ++var15) {
                        if (p >= 0 && p < 128) {
                            var16 = var1->m_Blocks[var14][var12][var15].getType();
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
                var12 = var1->m_Blocks[var7][var8 - 1][var9].getType();
                if ((var12 == BlockType::Grass || var12 == BlockType::Dirt) && var8 < 128 - var10 - 1) {
                    var1->m_Blocks[var7][var8 - 1][var9].setType(BlockType::Dirt);

                    for (int k = var8 - 3 + var10; k <= var8 + var10; ++k) {
                        var14 = k - (var8 + var10);
                        var15 = 1 - var14 / 2;

                        for (int j = var7 - var15; j <= var7 + var15; ++j) {
                            int var21 = j - var7;

                            for (int i = var9 - var15; i <= var9 + var15; ++i) {
                                int var17 = i - var9;
                                if ((std::abs(var21) != var15 || std::abs(var17) != var15 || var20.nextInt(2) != 0 && var14 != 0) && var1->m_Blocks[j][k][i].getData().visibility != BlockVisiblity::Opaque) {
                                    var1->m_Blocks[j][k][i].setType(BlockType::Glass);
                                }
                            }
                        }
                    }

                    for (int k = 0; k < var10; ++k) {
                        if (var1->m_Blocks[var7][var8 + k][var9].getData().visibility != BlockVisiblity::Opaque) {
                            var1->m_Blocks[var7][var8 + k][var9].setType(BlockType::Wood);
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




void Chunk::RenderOpaqueAndPlants() {
    Game::e_DefaultShader.use();
    Game::e_DefaultShader.setMat4("view", Game::View);
    Game::e_DefaultShader.setMat4("projection", Game::Proj);
    glm::vec3 relativePos = glm::dvec3(ChunkX * 16, 0, ChunkZ * 16) - Game::player.GetPosition();
    Game::e_DefaultShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));

    //enable rendering features for opaque blocks
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back faces
    glFrontFace(GL_CW); // Clockwise instead

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Indicies.size(), GL_UNSIGNED_INT, 0);

    //disable rendering features for plants
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(m_VAO2);
    glDrawElements(GL_TRIANGLES, m_Indicies2.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::RenderWater() {
    Game::e_WaterShader.use();
    Game::e_WaterShader.setMat4("view", Game::View);
    Game::e_WaterShader.setMat4("projection", Game::Proj);
    glm::vec3 relativePos = glm::dvec3(ChunkX * 16, 0, ChunkZ * 16) - Game::player.GetPosition();
    Game::e_WaterShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));

    glBindVertexArray(m_VAO3);
    glDrawElements(GL_TRIANGLES, m_Indicies3.size(), GL_UNSIGNED_INT, 0);
}


void Chunk::GenerateMesh() {
    m_Verticies.clear();
    m_Indicies.clear();

    m_Verticies2.clear();
    m_Indicies2.clear();

    m_Verticies3.clear();
    m_Indicies3.clear();
    uint32_t index = 0;
    uint32_t index1 = 0;
    uint32_t index2 = 0;

    for (int x = 0; x < Chunk_Width; ++x) {
        for (int y = 0; y < Chunk_Height; ++y) {
            for (int z = 0; z < Chunk_Length; ++z) {
                BlockType type = m_Blocks[x][y][z].m_Type;
                if (type == BlockType::Air)
                    continue;

                glm::u8vec3 blockPos(x, y, z);

                auto isAir = [&](int dx, int dy, int dz, BlockVisiblity visibility) -> bool { //for opaque blocks
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy, nz = z + dz;
                    if (nx < 0) { //check if outside on the X on the negative
                        if (owningLevel->IsValidChunk(ChunkX - 1, ChunkZ)) {
                            block = &owningLevel->getChunkAt(ChunkX - 1, ChunkZ)->m_Blocks[15][ny][nz];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }
                        return true;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        if (owningLevel->IsValidChunk(ChunkX + 1, ChunkZ)) {
                            block = &owningLevel->getChunkAt(ChunkX + 1, ChunkZ)->m_Blocks[0][ny][nz];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }
                        return true;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true; 
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        if (owningLevel->IsValidChunk(ChunkX, ChunkZ - 1)) {
                            block = &owningLevel->getChunkAt(ChunkX, ChunkZ - 1)->m_Blocks[nx][ny][15];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }
                        return true;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        if (owningLevel->IsValidChunk(ChunkX, ChunkZ + 1)) {
                            block = &owningLevel->getChunkAt(ChunkX, ChunkZ + 1)->m_Blocks[nx][ny][0];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }
                        return true;
                    }

                    //if inside the chunk
                    return m_Blocks[nx][ny][nz].getType() == BlockType::Air || m_Blocks[nx][ny][nz].data.visibility != visibility;
                    };
                auto isVis = [&](int dx, int dy, int dz, BlockVisiblity visibility)  -> bool {
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy, nz = z + dz;
                    if (nx < 0) { //check if outside on the X on the negative
                        if (owningLevel->IsValidChunk(ChunkX - 1, ChunkZ)) {
                            block = &owningLevel->getChunkAt(ChunkX - 1, ChunkZ)->m_Blocks[15][ny][nz];
                            return block->data.visibility == visibility;
                        }
                        return true;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        if (owningLevel->IsValidChunk(ChunkX + 1, ChunkZ)) {
                            block = &owningLevel->getChunkAt(ChunkX + 1, ChunkZ)->m_Blocks[0][ny][nz];
                            return block->data.visibility == visibility;
                        }
                        return true;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true;
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        if (owningLevel->IsValidChunk(ChunkX, ChunkZ - 1)) {
                            block = &owningLevel->getChunkAt(ChunkX, ChunkZ - 1)->m_Blocks[nx][ny][15];
                            return block->data.visibility == visibility;
                        }
                        return true;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        if (owningLevel->IsValidChunk(ChunkX, ChunkZ + 1)) {
                            block = &owningLevel->getChunkAt(ChunkX, ChunkZ + 1)->m_Blocks[nx][ny][0];
                            return block->data.visibility == visibility;
                        }
                        return true;
                    }

                    //if inside the chunk
                    return  m_Blocks[nx][ny][nz].data.visibility == visibility;
                    };

                switch (m_Blocks[x][y][z].data.visibility) { //basically add opaque blocks data
                case BlockVisiblity::Opaque:
                    if (isAir(0, 0, -1, Opaque)) AddFace(blockPos, Face::Back, m_Blocks[x][y][z].data.uv.Back, index);
                    if (isAir(0, 0, 1, Opaque)) AddFace(blockPos, Face::Front, m_Blocks[x][y][z].data.uv.Front, index);
                    if (isAir(-1, 0, 0, Opaque)) AddFace(blockPos, Face::Left, m_Blocks[x][y][z].data.uv.Left, index);
                    if (isAir(1, 0, 0, Opaque)) AddFace(blockPos, Face::Right, m_Blocks[x][y][z].data.uv.Right, index);
                    if (isAir(0, 1, 0, Opaque)) AddFace(blockPos, Face::Top, m_Blocks[x][y][z].data.uv.Top, index);
                    if (isAir(0, -1, 0, Opaque)) AddFace(blockPos, Face::Bottom, m_Blocks[x][y][z].data.uv.Bottom, index);
                    break;
                case BlockVisiblity::Plant:
                    AddPlantFace(blockPos, m_Blocks[x][y][z].data.uv.Back, index1);
                    break;
                case BlockVisiblity::Liquid:
                    if (isAir(0, 0, -1, Liquid)) AddLiquidFace(blockPos, Face::Back, m_Blocks[x][y][z].data.uv.Back, index2, isVis(0, 1, 0, Liquid));
                    if (isAir(0, 0, 1, Liquid)) AddLiquidFace(blockPos, Face::Front, m_Blocks[x][y][z].data.uv.Front, index2, isVis(0, 1, 0, Liquid));
                    if (isAir(-1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Left, m_Blocks[x][y][z].data.uv.Left, index2, isVis(0, 1, 0, Liquid));
                    if (isAir(1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Right, m_Blocks[x][y][z].data.uv.Right, index2, isVis(0, 1, 0, Liquid));
                    if (isAir(0, 1, 0, Liquid)) AddLiquidFace(blockPos, Face::Top, m_Blocks[x][y][z].data.uv.Top, index2, false);
                    if (isAir(0, -1, 0, Liquid)) AddLiquidFace(blockPos, Face::Bottom, m_Blocks[x][y][z].data.uv.Bottom, index2, false);
                    break;
                }
            }
        }
    }

    //Set Up opaque mesh
    if(m_Verticies.size() > 0)
    {
        // Upload to GPU
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Verticies.size() * sizeof(Vertex), m_Verticies.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicies.size() * sizeof(uint32_t), m_Indicies.data(), GL_STATIC_DRAW);

        // Vertex layout
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1); // uv
        glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }



    //Set Up plant mesh
    if (m_Verticies2.size() > 0)
    {
        // Upload to GPU
        glBindVertexArray(m_VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO2);
        glBufferData(GL_ARRAY_BUFFER, m_Verticies2.size() * sizeof(Vertex), m_Verticies2.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicies2.size() * sizeof(uint32_t), m_Indicies2.data(), GL_STATIC_DRAW);

        // Vertex layout
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }



    //Set Up liquid mesh
    if (m_Verticies3.size() > 0) {
        glBindVertexArray(m_VAO3);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO3);
        glBufferData(GL_ARRAY_BUFFER, m_Verticies3.size() * sizeof(Vertex), m_Verticies3.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO3);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicies3.size() * sizeof(uint32_t), m_Indicies3.data(), GL_STATIC_DRAW);

        // Vertex layout
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1); // uv
        glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasWater = true;
    }
    else HasWater = false;
}

UVQuad getUVQuad(int tileID, int atlasWidth, int atlasHeight) {
    float tileWidth = 1.0f / atlasWidth;
    float tileHeight = 1.0f / atlasHeight;

    int x = tileID % atlasWidth;
    int y = tileID / atlasWidth;

    // Slight offset to avoid border bleeding (half a texel)
    float pixelOffsetX = 0.5f / 256.0f;
    float pixelOffsetY = 0.5f / 256.0f;

    float u = x * tileWidth + pixelOffsetX;
    float v = y * tileHeight + pixelOffsetY;

    float u2 = (x + 1) * tileWidth - pixelOffsetX;
    float v2 = (y + 1) * tileHeight - pixelOffsetY;

    return UVQuad{
        glm::u8vec2(u * 256.0f, v * 256.0f),     // top-left
        glm::u8vec2(u2 * 256.0f, v * 256.0f),    // top-right
        glm::u8vec2(u2 * 256.0f, v2 * 256.0f),   // bottom-right
        glm::u8vec2(u * 256.0f, v2 * 256.0f)     // bottom-left
    };
}

//adds face for an opaque block
void Chunk::AddFace(glm::u8vec3 pos, Face face, int faceIndexOffset, uint32_t& indexOffset) {
    glm::u8vec3 p = pos;
    glm::u8vec3 v0, v1, v2, v3;
    glm::u8vec2 uv0, uv1, uv2, uv3;
    UVQuad uv = getUVQuad(faceIndexOffset, 16, 16); //calculates the UVs based on the index

    //this entire Switch basically sets the normal and verticies position based on the position of the face in question and the direction of the block
    switch (face) {
    case Face::Top:
        v0 = p + glm::u8vec3(0, 1, 0);
        v1 = p + glm::u8vec3(1, 1, 0);
        v2 = p + glm::u8vec3(1, 1, 1);
        v3 = p + glm::u8vec3(0, 1, 1);

        
        uv0 = uv.topLeft;
        uv1 = uv.topRight;
        uv2 = uv.bottomRight;
        uv3 = uv.bottomLeft;
        break;
    case Face::Bottom:
        v0 = p + glm::u8vec3(0, 0, 0);
        v1 = p + glm::u8vec3(0, 0, 1);
        v2 = p + glm::u8vec3(1, 0, 1);
        v3 = p + glm::u8vec3(1, 0, 0);

        uv0 = uv.bottomLeft;
        uv1 = uv.bottomRight;
        uv2 = uv.topRight;
        uv3 = uv.topLeft;
        break;
    case Face::Left:
        v0 = p + glm::u8vec3(0, 0, 0);
        v1 = p + glm::u8vec3(0, 1, 0);
        v2 = p + glm::u8vec3(0, 1, 1);
        v3 = p + glm::u8vec3(0, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Right:
        v0 = p + glm::u8vec3(1, 0, 0);
        v1 = p + glm::u8vec3(1, 0, 1);
        v2 = p + glm::u8vec3(1, 1, 1);
        v3 = p + glm::u8vec3(1, 1, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    case Face::Front:
        v0 = p + glm::u8vec3(0, 0, 1);
        v1 = p + glm::u8vec3(0, 1, 1);
        v2 = p + glm::u8vec3(1, 1, 1);
        v3 = p + glm::u8vec3(1, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Back:
        v0 = p + glm::u8vec3(0, 0, 0);
        v1 = p + glm::u8vec3(1, 0, 0);
        v2 = p + glm::u8vec3(1, 1, 0);
        v3 = p + glm::u8vec3(0, 1, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    }


    //adds the 4 verticies
    m_Verticies.push_back({ v0, uv0});
    m_Verticies.push_back({ v1, uv1 });
    m_Verticies.push_back({ v2, uv2 });
    m_Verticies.push_back({ v3, uv3});


    //adds the 6 indicies for the 4 verticies
    m_Indicies.push_back(indexOffset + 0);
    m_Indicies.push_back(indexOffset + 1);
    m_Indicies.push_back(indexOffset + 2);

    m_Indicies.push_back(indexOffset + 2);
    m_Indicies.push_back(indexOffset + 3);
    m_Indicies.push_back(indexOffset + 0);

    indexOffset += 4;
}
//adds face for a plant block
void Chunk::AddPlantFace(glm::u8vec3 pos, int faceIndexOffset, uint32_t& indexOffset) {
    glm::u8vec3 p = pos;
    glm::u8vec3 v0, v1, v2, v3;
    glm::u8vec2 uv0, uv1, uv2, uv3;

    UVQuad uv = getUVQuad(faceIndexOffset, 16, 16);


    // Diagonal Plane 1 (Northwest to Southeast)
    v0 = p + glm::u8vec3(0.0f, 1.0f, 0.0f); // top-left
    v1 = p + glm::u8vec3(1.0f, 1.0f, 1.0f); // top-right
    v2 = p + glm::u8vec3(1.0f, 0.0f, 1.0f); // bottom-right
    v3 = p + glm::u8vec3(0.0f, 0.0f, 0.0f); // bottom-left

    // First quad
    m_Verticies2.push_back({ v0, uv.topLeft });
    m_Verticies2.push_back({ v1, uv.topRight });
    m_Verticies2.push_back({ v2, uv.bottomRight });
    m_Verticies2.push_back({ v3, uv.bottomLeft });

    m_Indicies2.push_back(indexOffset + 0);
    m_Indicies2.push_back(indexOffset + 1);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 3);
    m_Indicies2.push_back(indexOffset + 0);

    indexOffset += 4;

    // Diagonal Plane 2 (Northeast to Southwest)
    v0 = p + glm::u8vec3(1.0f, 1.0f, 0.0f); // top-left
    v1 = p + glm::u8vec3(0.0f, 1.0f, 1.0f); // top-right
    v2 = p + glm::u8vec3(0.0f, 0.0f, 1.0f); // bottom-right
    v3 = p + glm::u8vec3(1.0f, 0.0f, 0.0f); // bottom-left

    // Second quad
    m_Verticies2.push_back({ v0, uv.topLeft });
    m_Verticies2.push_back({ v1, uv.topRight });
    m_Verticies2.push_back({ v2, uv.bottomRight });
    m_Verticies2.push_back({ v3, uv.bottomLeft });

    m_Indicies2.push_back(indexOffset + 0);
    m_Indicies2.push_back(indexOffset + 1);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 3);
    m_Indicies2.push_back(indexOffset + 0);

    indexOffset += 4;
}
//adds face for a liquid block
void Chunk::AddLiquidFace(glm::u8vec3 pos, Face face,int faceIndexOffset, uint32_t& indexOffset, bool IsMiddle) {
    glm::u8vec3 p = pos;
    glm::u8vec3 normal;
    glm::u8vec3 v0, v1, v2, v3;
    glm::u8vec2 uv0, uv1, uv2, uv3;
    UVQuad uv = getUVQuad(faceIndexOffset, 16, 16); //calculates the UVs based on the index

    //this entire Switch basically sets the normal and verticies position based on the position of the face in question and the direction of the block
    switch (face) {
    case Face::Top:
        v0 = p + glm::u8vec3(0, 0.8, 0);
        v1 = p + glm::u8vec3(1, 0.8, 0);
        v2 = p + glm::u8vec3(1, 0.8, 1);
        v3 = p + glm::u8vec3(0, 0.8, 1);


        uv0 = uv.topLeft;
        uv1 = uv.topRight;
        uv2 = uv.bottomRight;
        uv3 = uv.bottomLeft;
        break;
    case Face::Bottom:
        v0 = p + glm::u8vec3(0, 0, 0);
        v1 = p + glm::u8vec3(0, 0, 1);
        v2 = p + glm::u8vec3(1, 0, 1);
        v3 = p + glm::u8vec3(1, 0, 0);

        uv0 = uv.bottomLeft;
        uv1 = uv.bottomRight;
        uv2 = uv.topRight;
        uv3 = uv.topLeft;
        break;
    case Face::Left:
        v0 = p + glm::u8vec3(0, 0, 0);
        v1 = p + glm::u8vec3(0, IsMiddle ? 1 : 0.8, 0);
        v2 = p + glm::u8vec3(0, IsMiddle ? 1 : 0.8, 1);
        v3 = p + glm::u8vec3(0, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Right:
        v0 = p + glm::u8vec3(1, 0, 0);
        v1 = p + glm::u8vec3(1, 0, 1);
        v2 = p + glm::u8vec3(1, IsMiddle ? 1 : 0.8, 1);
        v3 = p + glm::u8vec3(1, IsMiddle ? 1 : 0.8, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    case Face::Front:
        v0 = p + glm::u8vec3(0, 0, 1);
        v1 = p + glm::u8vec3(0, IsMiddle ? 1 : 0.8, 1);
        v2 = p + glm::u8vec3(1, IsMiddle ? 1 : 0.8, 1);
        v3 = p + glm::u8vec3(1, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Back:
        v0 = p + glm::u8vec3(0, 0, 0);
        v1 = p + glm::u8vec3(1, 0, 0);
        v2 = p + glm::u8vec3(1, IsMiddle ? 1 : 0.8, 0);
        v3 = p + glm::u8vec3(0, IsMiddle ? 1 : 0.8, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    }


    //adds the 4 verticies
    m_Verticies3.push_back({ v0, uv0 });
    m_Verticies3.push_back({ v1, uv1 });
    m_Verticies3.push_back({ v2, uv2 });
    m_Verticies3.push_back({ v3, uv3 });


    //adds the 6 indicies for the 4 verticies
    m_Indicies3.push_back(indexOffset + 0);
    m_Indicies3.push_back(indexOffset + 1);
    m_Indicies3.push_back(indexOffset + 2);

    m_Indicies3.push_back(indexOffset + 2);
    m_Indicies3.push_back(indexOffset + 3);
    m_Indicies3.push_back(indexOffset + 0);

    indexOffset += 4;
}