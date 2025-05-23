#include "Chunk.h"
#include <glad/glad.h>
#include "Level.h"

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

//terrain generation.
void Chunk::GenerateChunk(NoiseGeneratorOctave* NoiseGen1, NoiseGeneratorOctave* NoiseGen2, NoiseGeneratorOctave* NoiseGen3,
    NoiseGeneratorOctave* NoiseGen4, NoiseGeneratorOctave* NoiseGen5, NoiseGeneratorOctave* NoiseGen6,
    Random& Rand) {
    for (int x = 0; x < Chunk_Width; x++) {
        for (int z = 0; z < Chunk_Length; z++) {
            int WorldX = x + ChunkX * 16;
            int WorldZ = z + ChunkZ * 16;

            int var7 = WorldX / 1024;
            int var8 = WorldZ / 1024;

            float var9 = (float)(NoiseGen1->generateNoise((double)((float)WorldX / 0.03125f), 0.0, (double)((float)WorldZ / 0.03125f)) - NoiseGen2->generateNoise((double)((float)WorldX / 0.015625f), 0.0, (double)((float)WorldZ / 0.015625f))) / 512.0f / 4.0f;
            float var10 = (float)NoiseGen5->generateNoise((double)((float)WorldX / 4.0f), (double)((float)WorldZ / 4.0f));
            float var11 = (float)NoiseGen6->generateNoise((double)((float)WorldX / 8.0f), (double)((float)WorldZ / 8.0f)) / 8.0f;

            var10 = var10 > 0.0f ? (float)(NoiseGen3->generateNoise((double)((float)WorldX * 0.25714284f * 2.0f), (double)((float)WorldZ * 0.25714284f * 2.0f)) * (double)var11 / 4.0) : (float)(NoiseGen4->generateNoise((double)((float)WorldX * 0.25714284f), (double)((float)WorldZ * 0.25714284f)) * (double)var11);
            int var15 = (int)(var9 + 64.0f + var10);
            if ((float)NoiseGen5->generateNoise((double)WorldX, (double)WorldZ) < 0.0f) {
                var15 = var15 / 2 << 1;
                if ((float)NoiseGen5->generateNoise((double)(WorldX / 5), (double)(WorldZ / 5)) < 0.0f) {
                    ++var15;
                }
            }


            for (int y = 0; y < Chunk_Height; y++) {
                m_Blocks[x][y][z].owningChunk = this;
                m_Blocks[x][y][z].BlockX = x;
                m_Blocks[x][y][z].BlockY = y;
                m_Blocks[x][y][z].BlockZ = z;

                double randomDouble = rand() / (RAND_MAX + 1.0);

                if ((WorldX == 0 || WorldZ == 0) && y <= var15 + 2) {
                    m_Blocks[x][y][z].setType(BlockType::Obsidian); //the obsidian cross in x = 0 and z = 0
                }
                else if (y == var15 + 1 && var15 >= 64 && randomDouble < 0.02) {
                    m_Blocks[x][y][z].setType(BlockType::YellowFlower);
                }
                else if (y == var15 && var15 >= 64) {
                    m_Blocks[x][y][z].setType(BlockType::Grass); //the grass layer
                }
                else if (y <= var15 - 2) {
                    m_Blocks[x][y][z].setType(BlockType::Stone); //the stone layers
                }
                else if (y <= var15) {
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

                if (var12 < var15) {
                    var12 = var15;
                }

                if (y <= var12 && (m_Blocks[x][y][z].getType() == BlockType::Air || m_Blocks[x][y][z].getType() == BlockType::WaterStill)) {
                    m_Blocks[x][y][z].setType(BlockType::Bricks);
                }
            }
        }
    }
}


void Chunk::RenderChunk() {
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

    //rendering features for the waters
    glEnable(GL_CULL_FACE);

    glBindVertexArray(m_VAO3);
    glDrawElements(GL_TRIANGLES, m_Indicies3.size(), GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
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

                glm::vec3 blockPos(x, y, z);

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

glm::vec2 getUV(int blockID) {
    const int atlasSize = 16; // 16x16 grid for texture atlas
    const float texWidth = 1.0f / atlasSize;
    const float texHeight = 1.0f / atlasSize;

    int row = blockID / atlasSize;  // Row in the atlas
    int col = blockID % atlasSize;  // Column in the atlas

    // Calculate the UV coordinates
    float xOffset = col * texWidth;
    float yOffset = row * texHeight;

    return glm::vec2(xOffset, yOffset);
}

UVQuad getUVQuad(int tileID, int atlasWidth, int atlasHeight) {
    float tileWidth = 1.0f / atlasWidth;
    float tileHeight = 1.0f / atlasHeight;

    int x = tileID % atlasWidth;
    int y = tileID / atlasWidth;

    float u = x * tileWidth;
    float v = y * tileHeight;

    return UVQuad{
        glm::vec2(u, v),                                 // top-left
        glm::vec2(u + tileWidth, v),                     // top-right
        glm::vec2(u + tileWidth, v + tileHeight),        // bottom-right
        glm::vec2(u, v + tileHeight)                     // bottom-left
    };
}

//adds face for an opaque block
void Chunk::AddFace(glm::vec3 pos, Face face, int faceIndexOffset, uint32_t& indexOffset) {
    glm::vec3 p = pos;
    glm::vec3 normal;
    glm::vec3 v0, v1, v2, v3;
    glm::vec2 uv0, uv1, uv2, uv3;
    UVQuad uv = getUVQuad(faceIndexOffset, 16, 16); //calculates the UVs based on the index

    //this entire Switch basically sets the normal and verticies position based on the position of the face in question and the direction of the block
    switch (face) {
    case Face::Top:
        normal = glm::vec3(0, 1, 0);
        v0 = p + glm::vec3(0, 1, 0);
        v1 = p + glm::vec3(1, 1, 0);
        v2 = p + glm::vec3(1, 1, 1);
        v3 = p + glm::vec3(0, 1, 1);

        
        uv0 = uv.topLeft;
        uv1 = uv.topRight;
        uv2 = uv.bottomRight;
        uv3 = uv.bottomLeft;
        break;
    case Face::Bottom:
        normal = glm::vec3(0, -1, 0);
        v0 = p + glm::vec3(0, 0, 0);
        v1 = p + glm::vec3(0, 0, 1);
        v2 = p + glm::vec3(1, 0, 1);
        v3 = p + glm::vec3(1, 0, 0);

        uv0 = uv.bottomLeft;
        uv1 = uv.bottomRight;
        uv2 = uv.topRight;
        uv3 = uv.topLeft;
        break;
    case Face::Left:
        normal = glm::vec3(-1, 0, 0);
        v0 = p + glm::vec3(0, 0, 0);
        v1 = p + glm::vec3(0, 1, 0);
        v2 = p + glm::vec3(0, 1, 1);
        v3 = p + glm::vec3(0, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Right:
        normal = glm::vec3(1, 0, 0);
        v0 = p + glm::vec3(1, 0, 0);
        v1 = p + glm::vec3(1, 0, 1);
        v2 = p + glm::vec3(1, 1, 1);
        v3 = p + glm::vec3(1, 1, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    case Face::Front:
        normal = glm::vec3(0, 0, 1);
        v0 = p + glm::vec3(0, 0, 1);
        v1 = p + glm::vec3(0, 1, 1);
        v2 = p + glm::vec3(1, 1, 1);
        v3 = p + glm::vec3(1, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Back:
        normal = glm::vec3(0, 0, -1);
        v0 = p + glm::vec3(0, 0, 0);
        v1 = p + glm::vec3(1, 0, 0);
        v2 = p + glm::vec3(1, 1, 0);
        v3 = p + glm::vec3(0, 1, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    }


    //adds the 4 verticies
    m_Verticies.push_back({ v0, normal, uv0});
    m_Verticies.push_back({ v1, normal, uv1 });
    m_Verticies.push_back({ v2, normal, uv2 });
    m_Verticies.push_back({ v3, normal, uv3});


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
void Chunk::AddPlantFace(glm::vec3 pos, int faceIndexOffset, uint32_t& indexOffset) {
    glm::vec3 p = pos;
    glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 v0, v1, v2, v3;
    glm::vec2 uv0, uv1, uv2, uv3;

    UVQuad uv = getUVQuad(faceIndexOffset, 16, 16);


    // Diagonal Plane 1 (Northwest to Southeast)
    v0 = p + glm::vec3(0.0f, 1.0f, 0.0f); // top-left
    v1 = p + glm::vec3(1.0f, 1.0f, 1.0f); // top-right
    v2 = p + glm::vec3(1.0f, 0.0f, 1.0f); // bottom-right
    v3 = p + glm::vec3(0.0f, 0.0f, 0.0f); // bottom-left

    // First quad
    m_Verticies2.push_back({ v0, normal, uv.topLeft });
    m_Verticies2.push_back({ v1, normal, uv.topRight });
    m_Verticies2.push_back({ v2, normal, uv.bottomRight });
    m_Verticies2.push_back({ v3, normal, uv.bottomLeft });

    m_Indicies2.push_back(indexOffset + 0);
    m_Indicies2.push_back(indexOffset + 1);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 3);
    m_Indicies2.push_back(indexOffset + 0);

    indexOffset += 4;

    // Diagonal Plane 2 (Northeast to Southwest)
    v0 = p + glm::vec3(1.0f, 1.0f, 0.0f); // top-left
    v1 = p + glm::vec3(0.0f, 1.0f, 1.0f); // top-right
    v2 = p + glm::vec3(0.0f, 0.0f, 1.0f); // bottom-right
    v3 = p + glm::vec3(1.0f, 0.0f, 0.0f); // bottom-left

    // Second quad
    m_Verticies2.push_back({ v0, normal, uv.topLeft });
    m_Verticies2.push_back({ v1, normal, uv.topRight });
    m_Verticies2.push_back({ v2, normal, uv.bottomRight });
    m_Verticies2.push_back({ v3, normal, uv.bottomLeft });

    m_Indicies2.push_back(indexOffset + 0);
    m_Indicies2.push_back(indexOffset + 1);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 2);
    m_Indicies2.push_back(indexOffset + 3);
    m_Indicies2.push_back(indexOffset + 0);

    indexOffset += 4;
}
//adds face for a liquid block
void Chunk::AddLiquidFace(glm::vec3 pos, Face face,int faceIndexOffset, uint32_t& indexOffset, bool IsMiddle) {
    glm::vec3 p = pos;
    glm::vec3 normal;
    glm::vec3 v0, v1, v2, v3;
    glm::vec2 uv0, uv1, uv2, uv3;
    UVQuad uv = getUVQuad(faceIndexOffset, 16, 16); //calculates the UVs based on the index

    //this entire Switch basically sets the normal and verticies position based on the position of the face in question and the direction of the block
    switch (face) {
    case Face::Top:
        normal = glm::vec3(0, 1, 0);
        v0 = p + glm::vec3(0, 0.8, 0);
        v1 = p + glm::vec3(1, 0.8, 0);
        v2 = p + glm::vec3(1, 0.8, 1);
        v3 = p + glm::vec3(0, 0.8, 1);


        uv0 = uv.topLeft;
        uv1 = uv.topRight;
        uv2 = uv.bottomRight;
        uv3 = uv.bottomLeft;
        break;
    case Face::Bottom:
        normal = glm::vec3(0, -1, 0);
        v0 = p + glm::vec3(0, 0, 0);
        v1 = p + glm::vec3(0, 0, 1);
        v2 = p + glm::vec3(1, 0, 1);
        v3 = p + glm::vec3(1, 0, 0);

        uv0 = uv.bottomLeft;
        uv1 = uv.bottomRight;
        uv2 = uv.topRight;
        uv3 = uv.topLeft;
        break;
    case Face::Left:
        normal = glm::vec3(-1, 0, 0);
        v0 = p + glm::vec3(0, 0, 0);
        v1 = p + glm::vec3(0, IsMiddle ? 1 : 0.8, 0);
        v2 = p + glm::vec3(0, IsMiddle ? 1 : 0.8, 1);
        v3 = p + glm::vec3(0, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Right:
        normal = glm::vec3(1, 0, 0);
        v0 = p + glm::vec3(1, 0, 0);
        v1 = p + glm::vec3(1, 0, 1);
        v2 = p + glm::vec3(1, IsMiddle ? 1 : 0.8, 1);
        v3 = p + glm::vec3(1, IsMiddle ? 1 : 0.8, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    case Face::Front:
        normal = glm::vec3(0, 0, 1);
        v0 = p + glm::vec3(0, 0, 1);
        v1 = p + glm::vec3(0, IsMiddle ? 1 : 0.8, 1);
        v2 = p + glm::vec3(1, IsMiddle ? 1 : 0.8, 1);
        v3 = p + glm::vec3(1, 0, 1);

        uv0 = uv.bottomLeft;
        uv1 = uv.topLeft;
        uv2 = uv.topRight;
        uv3 = uv.bottomRight;
        break;
    case Face::Back:
        normal = glm::vec3(0, 0, -1);
        v0 = p + glm::vec3(0, 0, 0);
        v1 = p + glm::vec3(1, 0, 0);
        v2 = p + glm::vec3(1, IsMiddle ? 1 : 0.8, 0);
        v3 = p + glm::vec3(0, IsMiddle ? 1 : 0.8, 0);

        uv0 = uv.bottomRight;
        uv1 = uv.bottomLeft;
        uv2 = uv.topLeft;
        uv3 = uv.topRight;
        break;
    }


    //adds the 4 verticies
    m_Verticies3.push_back({ v0, normal, uv0 });
    m_Verticies3.push_back({ v1, normal, uv1 });
    m_Verticies3.push_back({ v2, normal, uv2 });
    m_Verticies3.push_back({ v3, normal, uv3 });


    //adds the 6 indicies for the 4 verticies
    m_Indicies3.push_back(indexOffset + 0);
    m_Indicies3.push_back(indexOffset + 1);
    m_Indicies3.push_back(indexOffset + 2);

    m_Indicies3.push_back(indexOffset + 2);
    m_Indicies3.push_back(indexOffset + 3);
    m_Indicies3.push_back(indexOffset + 0);

    indexOffset += 4;
}