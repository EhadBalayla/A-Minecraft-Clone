#include "Chunk.h"
#include <glad/glad.h>
#include "Level.h"

Chunk::Chunk() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}
Chunk::~Chunk() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

int Chunk::DistanceFromChunk(Chunk* chunk) { //returns the distance from a given chunk, only on a single axis tho, this is generally used for render distance
    int DistanceX = std::abs(std::abs(chunk->ChunkX) - std::abs(ChunkX));
    int DistanceZ = std::abs(std::abs(chunk->ChunkZ) - std::abs(ChunkZ));
    if (DistanceX > DistanceZ)
        return DistanceX;
    return DistanceZ;
}

void Chunk::GenerateChunk(CombinedNoise& var6, CombinedNoise& var7, OctaveNoise& var8, OctaveNoise& var54) {
    for (int x = 0; x < Chunk_Width; x++) {
        for (int z = 0; z < Chunk_Length; z++) {
            int worldX = ChunkX * Chunk_Width + x;
            int worldZ = ChunkZ * Chunk_Length + z;


            float nx = worldX * 1.3;
            float nz = worldZ * 1.3;

            double lowland = var6.compute(nx, nz) / 6.0 - 4.0;
            double highland = var7.compute(nx, nz) / 5.0 + 6.0;

            if (var8.compute(worldX, worldZ) / 8.0 > 0.0)
                highland = lowland;

            float base = std::max(lowland, highland) / 2.0;
            if (base < 0)
                base *= 0.8;


            int intHeight = (int)base + 32 + var54.compute(worldX, worldZ) / 24 - 4;


            for (int y = 0; y < Chunk_Height; y++) {
                m_Blocks[x][y][z].owningChunk = this;
                m_Blocks[x][y][z].BlockX = x;
                m_Blocks[x][y][z].BlockY = y;
                m_Blocks[x][y][z].BlockZ = z;

                if (y == intHeight) {
                    if (y < 25)
                        m_Blocks[x][y][z].setType(BlockType::Sand);
                    else
                        m_Blocks[x][y][z].setType(BlockType::Grass);
                }
                else if (y < intHeight && y > intHeight - 5) {
                    m_Blocks[x][y][z].setType(BlockType::Dirt);
                }
                else if (y <= intHeight - 5) {
                    m_Blocks[x][y][z].setType(BlockType::Stone);
                }
                else {
                    m_Blocks[x][y][z].setType(BlockType::Air);
                }
            }
        }
    }
}


void Chunk::RenderChunk() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Indicies.size(), GL_UNSIGNED_INT, 0);
}

void Chunk::GenerateMesh() {
    m_Verticies.clear();
    m_Indicies.clear();
    uint32_t index = 0;

    for (int x = 0; x < Chunk_Width; ++x) {
        for (int y = 0; y < Chunk_Height; ++y) {
            for (int z = 0; z < Chunk_Length; ++z) {
                BlockType type = m_Blocks[x][y][z].m_Type;
                if (type == BlockType::Air)
                    continue;

                glm::vec3 blockPos(x, y, z);

                auto isAir = [&](int dx, int dy, int dz) -> bool {
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy, nz = z + dz;
                    if (nx < 0) { //check if outside on the X on the negative
                        if (owningLevel->IsValidChunk(ChunkX - 1, ChunkZ)) {
                            return owningLevel->getChunkAt(ChunkX - 1, ChunkZ)->m_Blocks[15][ny][nz].getType() == BlockType::Air;
                        }
                        return true;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        if (owningLevel->IsValidChunk(ChunkX + 1, ChunkZ)) {
                            return owningLevel->getChunkAt(ChunkX + 1, ChunkZ)->m_Blocks[0][ny][nz].getType() == BlockType::Air;
                        }
                        return true;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true; 
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        if (owningLevel->IsValidChunk(ChunkX, ChunkZ - 1)) {
                            return owningLevel->getChunkAt(ChunkX, ChunkZ - 1)->m_Blocks[nx][ny][15].getType() == BlockType::Air;
                        }
                        return true;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        if (owningLevel->IsValidChunk(ChunkX, ChunkZ + 1)) {
                            return owningLevel->getChunkAt(ChunkX, ChunkZ + 1)->m_Blocks[nx][ny][0].getType() == BlockType::Air;
                        }
                        return true;
                    }

                    //if inside the chunk
                    return m_Blocks[nx][ny][nz].getType() == BlockType::Air;
                    };

                // If face is exposed, add its vertices
                if (isAir(0, 0, -1)) AddFace(blockPos, Face::Back, m_Blocks[x][y][z].FaceUVs.Back, index);
                if (isAir(0, 0, 1)) AddFace(blockPos, Face::Front, m_Blocks[x][y][z].FaceUVs.Front, index);
                if (isAir(-1, 0, 0)) AddFace(blockPos, Face::Left, m_Blocks[x][y][z].FaceUVs.Left, index);
                if (isAir(1, 0, 0)) AddFace(blockPos, Face::Right, m_Blocks[x][y][z].FaceUVs.Right, index);
                if (isAir(0, 1, 0)) AddFace(blockPos, Face::Top, m_Blocks[x][y][z].FaceUVs.Top, index);
                if (isAir(0, -1, 0)) AddFace(blockPos, Face::Bottom, m_Blocks[x][y][z].FaceUVs.Bottom, index);
            }
        }
    }

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