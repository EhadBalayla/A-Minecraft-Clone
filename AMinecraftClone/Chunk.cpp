#include "Chunk.h"
#include <glad/glad.h>
#include "Level.h"
#include "MathHelper.h"
#include "Game.h"

void AddFace(glm::u8vec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies);
void AddPlantFace(glm::u8vec3 pos, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies);
void AddLiquidFace(glm::u8vec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, bool IsMiddle, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies);


Chunk::Chunk() {
}
Chunk::~Chunk() {

}

int Chunk::DistanceFromChunk(Chunk* chunk) { //returns the distance from a given chunk, only on a single axis tho, this is generally used for render distance
    int DistanceX = std::abs(std::abs(chunk->ChunkX) - std::abs(ChunkX));
    int DistanceZ = std::abs(std::abs(chunk->ChunkZ) - std::abs(ChunkZ));
    if (DistanceX > DistanceZ)
        return DistanceX;
    return DistanceZ;
}


void Chunk::CreateMeshObjects() {
    //generate buffer objects for opaque
    for (uint8_t i = 0; i < 5; i++) {
        glGenVertexArrays(1, &meshes[i].m_VAO);
        glGenBuffers(1, &meshes[i].m_VBO);
        glGenBuffers(1, &meshes[i].m_EBO);

        //generate buffer objects for plants
        glGenVertexArrays(1, &meshes[i].m_VAO2);
        glGenBuffers(1, &meshes[i].m_VBO2);
        glGenBuffers(1, &meshes[i].m_EBO2);

        //generate buffer objects for liquids
        glGenVertexArrays(1, &meshes[i].m_VAO3);
        glGenBuffers(1, &meshes[i].m_VBO3);
        glGenBuffers(1, &meshes[i].m_EBO3);
    }
}
void Chunk::DeleteMeshObjects() {
    for (uint8_t i = 0; i < 5; i++) {
        //delete buffer objects for opaque
        glDeleteVertexArrays(1, &meshes[i].m_VAO);
        glDeleteBuffers(1, &meshes[i].m_VBO);
        glDeleteBuffers(1, &meshes[i].m_EBO);

        //delete buffer objects for plants
        glDeleteVertexArrays(1, &meshes[i].m_VAO2);
        glDeleteBuffers(1, &meshes[i].m_VBO2);
        glDeleteBuffers(1, &meshes[i].m_EBO2);

        //delete buffer objects for liquids
        glDeleteVertexArrays(1, &meshes[i].m_VAO3);
        glDeleteBuffers(1, &meshes[i].m_VBO3);
        glDeleteBuffers(1, &meshes[i].m_EBO3);
    }
}



void Chunk::RenderOpaqueAndPlants() {
    Game::e_DefaultShader.use();
    Game::e_DefaultShader.setMat4("view", Game::View);
    Game::e_DefaultShader.setMat4("projection", Game::Proj);
    glm::vec3 relativePos = glm::dvec3(ChunkX * 16, 0, ChunkZ * 16) - Game::player.GetPosition();
    Game::e_DefaultShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));
    Game::e_DefaultShader.setUInt("LODFactor", 1 << LOD);

    //enable rendering features for opaque blocks
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back faces
    glFrontFace(GL_CW); // Clockwise instead

    glBindVertexArray(meshes[LOD].m_VAO);
    glDrawElements(GL_TRIANGLES, meshes[LOD].opaqueCount, GL_UNSIGNED_INT, 0);

    //disable rendering features for plants
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(meshes[LOD].m_VAO2);
    glDrawElements(GL_TRIANGLES, meshes[LOD].plantCount, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
}
void Chunk::RenderWater() {
    Game::e_WaterShader.use();
    Game::e_WaterShader.setMat4("view", Game::View);
    Game::e_WaterShader.setMat4("projection", Game::Proj);
    glm::vec3 relativePos = glm::dvec3(ChunkX * 16, 0, ChunkZ * 16) - Game::player.GetPosition();
    Game::e_WaterShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));
    Game::e_WaterShader.setFloat("Time", SDL_GetTicks() / 1000.0f);
    Game::e_WaterShader.setUInt("LODFactor", 1 << LOD);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    glBindVertexArray(meshes[LOD].m_VAO3);
    glDrawElements(GL_TRIANGLES, meshes[LOD].waterCount, GL_UNSIGNED_INT, 0);
}


ChunkMeshUpload CreateChunkMeshData(Chunk& chunk, uint8_t LOD) {
    uint8_t LODFactor = 1 << LOD;

    uint32_t index = 0;
    uint32_t index1 = 0;
    uint32_t index2 = 0;

    ChunkMeshUpload ret{};

    for (int x = 0; x < Chunk_Width; x += LODFactor) {
        for (int y = 0; y < Chunk_Height; y += LODFactor) {
            for (int z = 0; z < Chunk_Length; z += LODFactor) {
                BlockType type = chunk.m_Blocks[x][y][z].m_Type;
                if (type == BlockType::Air)
                    continue;

                glm::u8vec3 blockPos(x, y, z);

                auto isAir = [&](int dx, int dy, int dz, BlockVisiblity visibility) -> bool { //for opaque blocks
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + (dx * LODFactor), ny = y + (dy * LODFactor), nz = z + (dz * LODFactor);
                    if (nx < 0) { //check if outside on the X on the negative
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX - 1, chunk.ChunkZ)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX - 1, chunk.ChunkZ)->m_Blocks[15][ny][nz];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }*/
                        return true;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX + 1, chunk.ChunkZ)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX + 1, chunk.ChunkZ)->m_Blocks[0][ny][nz];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }*/
                        return true;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true;
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX, chunk.ChunkZ - 1)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX, chunk.ChunkZ - 1)->m_Blocks[nx][ny][15];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }*/
                        return true;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX, chunk.ChunkZ + 1)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX, chunk.ChunkZ + 1)->m_Blocks[nx][ny][0];
                            return block->getType() == BlockType::Air || block->data.visibility != visibility;
                        }*/
                        return true;
                    }

                    //if inside the chunk
                    return chunk.m_Blocks[nx][ny][nz].getType() == BlockType::Air || chunk.m_Blocks[nx][ny][nz].data.visibility != visibility;
                    };
                auto isVis = [&](int dx, int dy, int dz, BlockVisiblity visibility)  -> bool {
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy, nz = z + dz;
                    if (nx < 0) { //check if outside on the X on the negative
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX - 1, chunk.ChunkZ)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX - 1, chunk.ChunkZ)->m_Blocks[15][ny][nz];
                            return block->data.visibility == visibility;
                        }*/
                        return true;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX + 1, chunk.ChunkZ)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX + 1, chunk.ChunkZ)->m_Blocks[0][ny][nz];
                            return block->data.visibility == visibility;
                        }*/
                        return true;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true;
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX, chunk.ChunkZ - 1)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX, chunk.ChunkZ - 1)->m_Blocks[nx][ny][15];
                            return block->data.visibility == visibility;
                        }*/
                        return true;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        /*if (chunk.owningWorld->IsValidChunk(chunk.ChunkX, chunk.ChunkZ + 1)) {
                            block = &chunk.owningWorld->getChunkAt(chunk.ChunkX, chunk.ChunkZ + 1)->m_Blocks[nx][ny][0];
                            return block->data.visibility == visibility;
                        }*/
                        return true;
                    }

                    //if inside the chunk
                    return  chunk.m_Blocks[nx][ny][nz].data.visibility == visibility;
                    };

                switch (chunk.m_Blocks[x][y][z].data.visibility) {
                case BlockVisiblity::Opaque:
                    if (isAir(0, 0, -1, Opaque)) AddFace(blockPos, Face::Back, chunk.m_Blocks[x][y][z].data.uv.Back, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, 0, 1, Opaque)) AddFace(blockPos, Face::Front, chunk.m_Blocks[x][y][z].data.uv.Front, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(-1, 0, 0, Opaque)) AddFace(blockPos, Face::Left, chunk.m_Blocks[x][y][z].data.uv.Left, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(1, 0, 0, Opaque)) AddFace(blockPos, Face::Right, chunk.m_Blocks[x][y][z].data.uv.Right, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, 1, 0, Opaque)) AddFace(blockPos, Face::Top, chunk.m_Blocks[x][y][z].data.uv.Top, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, -1, 0, Opaque)) AddFace(blockPos, Face::Bottom, chunk.m_Blocks[x][y][z].data.uv.Bottom, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    break;
                case BlockVisiblity::Plant:
                    if(LOD == 0)
                        AddPlantFace(blockPos, chunk.m_Blocks[x][y][z].data.uv.Back, index1, ret.plantVerticies, ret.plantIndicies);
                    break;
                case BlockVisiblity::Liquid:
                    if (isAir(0, 0, -1, Liquid)) AddLiquidFace(blockPos, Face::Back, chunk.m_Blocks[x][y][z].data.uv.Back, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, 0, 1, Liquid)) AddLiquidFace(blockPos, Face::Front, chunk.m_Blocks[x][y][z].data.uv.Front, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(-1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Left, chunk.m_Blocks[x][y][z].data.uv.Left, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Right, chunk.m_Blocks[x][y][z].data.uv.Right, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, 1, 0, Liquid)) AddLiquidFace(blockPos, Face::Top, chunk.m_Blocks[x][y][z].data.uv.Top, index2, false, ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, -1, 0, Liquid)) AddLiquidFace(blockPos, Face::Bottom, chunk.m_Blocks[x][y][z].data.uv.Bottom, index2, false, ret.waterVerticies, ret.waterIndicies);
                    break;
                }
            }
        }
    }

    return ret;
}
void Chunk::ChunkUpload(ChunkMeshUpload& meshData, uint8_t LOD) {
    if (meshData.opaqueVerticies.size() > 0)
    {
        // Upload to GPU
        glBindVertexArray(meshes[LOD].m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshes[LOD].m_VBO);
        glBufferData(GL_ARRAY_BUFFER, meshData.opaqueVerticies.size() * sizeof(Vertex), meshData.opaqueVerticies.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[LOD].m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.opaqueIndicies.size() * sizeof(uint32_t), meshData.opaqueIndicies.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        meshes[LOD].opaqueCount = meshData.opaqueIndicies.size();
    }



    //Set Up plant mesh
    if (meshData.plantVerticies.size() > 0)
    {
        // Upload to GPU
        glBindVertexArray(meshes[LOD].m_VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, meshes[LOD].m_VBO2);
        glBufferData(GL_ARRAY_BUFFER, meshData.plantVerticies.size() * sizeof(Vertex), meshData.plantVerticies.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[LOD].m_EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.plantIndicies.size() * sizeof(uint32_t), meshData.plantIndicies.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        meshes[LOD].plantCount = meshData.plantIndicies.size();
    }



    //Set Up liquid mesh
    if (meshData.waterVerticies.size() > 0) {
        glBindVertexArray(meshes[LOD].m_VAO3);
        glBindBuffer(GL_ARRAY_BUFFER, meshes[LOD].m_VBO3);
        glBufferData(GL_ARRAY_BUFFER, meshData.waterVerticies.size() * sizeof(Vertex), meshData.waterVerticies.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[LOD].m_EBO3);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.waterIndicies.size() * sizeof(uint32_t), meshData.waterIndicies.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasWater = true;
        meshes[LOD].waterCount = meshData.waterIndicies.size();
    }
    else HasWater = false;
}


//helper functions
uint16_t convertPos(glm::u8vec3 pos) {
    uint16_t ret = pos.y << 8;
    ret |= pos.z << 4;
    ret |= pos.x;
    return ret;
}
uint8_t getExtras(uint8_t texCorner, uint8_t blockCorner) {
    uint8_t ret = blockCorner << 2;
    ret |= texCorner;
    return ret;
}
void waterFlag(uint16_t& pos, uint8_t& extras, uint16_t flag /*from 1 - 10*/) {
    pos = (pos & 0x7FFF) | ((flag & 1) << 15);
    extras = (extras & 0x1F) | ((flag >> 1 & 0x7) << 5);
}

//adds face for an opaque block
void AddFace(glm::u8vec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies) {
    uint8_t e0 = 0, e1 = 0, e2 = 0, e3 = 0;

    //this entire Switch basically sets the normal and verticies position based on the position of the face in question and the direction of the block
    switch (face) {
    case Face::Top:
        e0 = getExtras(0, 4);
        e1 = getExtras(1, 5);
        e2 = getExtras(2, 6);
        e3 = getExtras(3, 7);
        break;
    case Face::Bottom:
        e0 = getExtras(3, 0);
        e1 = getExtras(2, 3);
        e2 = getExtras(1, 2);
        e3 = getExtras(0, 1);
        break;
    case Face::Left:
        e0 = getExtras(3, 0);
        e1 = getExtras(0, 4);
        e2 = getExtras(1, 7);
        e3 = getExtras(2, 3);
        break;
    case Face::Right:
        e0 = getExtras(2, 1);
        e1 = getExtras(3, 2);
        e2 = getExtras(0, 6);
        e3 = getExtras(1, 5);
        break;
    case Face::Front:
        e0 = getExtras(3, 3);
        e1 = getExtras(0, 7);
        e2 = getExtras(1, 6);
        e3 = getExtras(2, 2);
        break;
    case Face::Back:
        e0 = getExtras(2, 0);
        e1 = getExtras(3, 1);
        e2 = getExtras(0, 5);
        e3 = getExtras(1, 4);
        break;
    }


    //adds the 4 verticies
    verticies.push_back({ convertPos(pos), faceIndexOffset, e0 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e1 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e2 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e3 });


    //adds the 6 indicies for the 4 verticies
    indicies.push_back(indexOffset + 0);
    indicies.push_back(indexOffset + 1);
    indicies.push_back(indexOffset + 2);

    indicies.push_back(indexOffset + 2);
    indicies.push_back(indexOffset + 3);
    indicies.push_back(indexOffset + 0);

    indexOffset += 4;
}
//adds face for a plant block
void AddPlantFace(glm::u8vec3 pos, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies) {
    uint8_t e0 = 0, e1 = 0, e2 = 0, e3 = 0;


    e0 = getExtras(0, 4);
    e1 = getExtras(1, 6);
    e2 = getExtras(2, 2);
    e3 = getExtras(3, 0);

    // First quad
    verticies.push_back({ convertPos(pos), faceIndexOffset, e0 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e1 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e2 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e3 });

    indicies.push_back(indexOffset + 0);
    indicies.push_back(indexOffset + 1);
    indicies.push_back(indexOffset + 2);
    indicies.push_back(indexOffset + 2);
    indicies.push_back(indexOffset + 3);
    indicies.push_back(indexOffset + 0);

    indexOffset += 4;

    // Diagonal Plane 2 (Northeast to Southwest)

    e0 = getExtras(0, 5);
    e1 = getExtras(1, 7);
    e2 = getExtras(2, 3);
    e3 = getExtras(3, 1);

    // Second quad
    verticies.push_back({ convertPos(pos), faceIndexOffset, e0 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e1 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e2 });
    verticies.push_back({ convertPos(pos), faceIndexOffset, e3 });

    indicies.push_back(indexOffset + 0);
    indicies.push_back(indexOffset + 1);
    indicies.push_back(indexOffset + 2);
    indicies.push_back(indexOffset + 2);
    indicies.push_back(indexOffset + 3);
    indicies.push_back(indexOffset + 0);

    indexOffset += 4;
}
//adds face for a liquid block
void AddLiquidFace(glm::u8vec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, bool IsMiddle, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies) {
    uint8_t e0 = 0, e1 = 0, e2 = 0, e3 = 0;
    uint16_t p0 = convertPos(pos), p1 = convertPos(pos), p2 = convertPos(pos), p3 = convertPos(pos);

    //this entire Switch basically sets the normal and verticies position based on the position of the face in question and the direction of the block
    switch (face) {
    case Face::Top:
        e0 = getExtras(0, 4);
        e1 = getExtras(1, 5);
        e2 = getExtras(2, 6);
        e3 = getExtras(3, 7);

        waterFlag(p0, e0, 8);
        waterFlag(p1, e1, 8);
        waterFlag(p2, e2, 8);
        waterFlag(p3, e3, 8);
        break;
    case Face::Bottom:
        e0 = getExtras(3, 0);
        e1 = getExtras(2, 3);
        e2 = getExtras(1, 2);
        e3 = getExtras(0, 1);

        waterFlag(p0, e0, 10);
        waterFlag(p1, e1, 10);
        waterFlag(p2, e2, 10);
        waterFlag(p3, e3, 10);
        break;
    case Face::Left:
        e0 = getExtras(3, 0);
        e1 = getExtras(1, 4);
        e2 = getExtras(0, 7);
        e3 = getExtras(2, 3);

        waterFlag(p0, e0, 10);
        waterFlag(p1, e1, IsMiddle ? 10 : 8);
        waterFlag(p2, e2, IsMiddle ? 10 : 8);
        waterFlag(p3, e3, 10);
        break;
    case Face::Right:
        e0 = getExtras(2, 1);
        e1 = getExtras(3, 2);
        e2 = getExtras(0, 6);
        e3 = getExtras(1, 5);

        waterFlag(p0, e0, 10);
        waterFlag(p1, e1, 10);
        waterFlag(p2, e2, IsMiddle ? 10 : 8);
        waterFlag(p3, e3, IsMiddle ? 10 : 8);
        break;
    case Face::Front:
        e0 = getExtras(3, 3);
        e1 = getExtras(0, 7);
        e2 = getExtras(1, 6);
        e3 = getExtras(2, 2);

        waterFlag(p0, e0, 10);
        waterFlag(p1, e1, IsMiddle ? 10 : 8);
        waterFlag(p2, e2, IsMiddle ? 10 : 8);
        waterFlag(p3, e3, 10);
        break;
    case Face::Back:
        e0 = getExtras(2, 0);
        e1 = getExtras(3, 1);
        e2 = getExtras(0, 5);
        e3 = getExtras(1, 4);

        waterFlag(p0, e0, 10);
        waterFlag(p1, e1, 10);
        waterFlag(p2, e2, IsMiddle ? 10 : 8);
        waterFlag(p3, e3, IsMiddle ? 10 : 8);
        break;
    }


    //adds the 4 verticies
    verticies.push_back({ p0, faceIndexOffset, e0 });
    verticies.push_back({ p1, faceIndexOffset, e1 });
    verticies.push_back({ p2, faceIndexOffset, e2 });
    verticies.push_back({ p3, faceIndexOffset, e3 });


    //adds the 6 indicies for the 4 verticies
    indicies.push_back(indexOffset + 0);
    indicies.push_back(indexOffset + 1);
    indicies.push_back(indexOffset + 2);

    indicies.push_back(indexOffset + 2);
    indicies.push_back(indexOffset + 3);
    indicies.push_back(indexOffset + 0);

    indexOffset += 4;
}