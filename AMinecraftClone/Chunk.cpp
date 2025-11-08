#include "Chunk.h"
#include <glad/glad.h>
#include "Level.h"
#include "MathHelper.h"
#include "Game.h"

void AddFace(glm::uvec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies);
void AddPlantFace(glm::uvec3 pos, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies);
void AddLiquidFace(glm::uvec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, bool IsMiddle, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies);


void Chunk::CreateMeshObjects() {
    //generate buffer objects for opaque
    glGenVertexArrays(1, &meshes.m_VAO);
    glGenBuffers(1, &meshes.m_VBO);
    glGenBuffers(1, &meshes.m_EBO);

    //generate buffer objects for plants
    glGenVertexArrays(1, &meshes.m_VAO2);
    glGenBuffers(1, &meshes.m_VBO2);
    glGenBuffers(1, &meshes.m_EBO2);

    //generate buffer objects for liquids
    glGenVertexArrays(1, &meshes.m_VAO3);
    glGenBuffers(1, &meshes.m_VBO3);
    glGenBuffers(1, &meshes.m_EBO3);
}
void Chunk::DeleteMeshObjects() {
    //delete buffer objects for opaque
    glDeleteVertexArrays(1, &meshes.m_VAO);
    glDeleteBuffers(1, &meshes.m_VBO);
    glDeleteBuffers(1, &meshes.m_EBO);

    //delete buffer objects for plants
    glDeleteVertexArrays(1, &meshes.m_VAO2);
    glDeleteBuffers(1, &meshes.m_VBO2);
    glDeleteBuffers(1, &meshes.m_EBO2);

    //delete buffer objects for liquids
    glDeleteVertexArrays(1, &meshes.m_VAO3);
    glDeleteBuffers(1, &meshes.m_VBO3);
    glDeleteBuffers(1, &meshes.m_EBO3);
}

void Chunk::GenerateHeightMap() {
    int var1 = 127;

    for (int var2 = 0; var2 < Chunk_Width; ++var2) {
        for (int var3 = 0; var3 < Chunk_Length; ++var3) {
            int var4 = 127;

            for (int var5 = var2 << 11 | var3 << 7; var4 > 0 /* && Block.lightOpacity[this.blocks[var5 + var4 - 1]] == 0*/; --var4) {
            }

            HeightMap[var3 << 4 | var2] = var4;
            if (var4 < var1) {
                var1 = var4;
            }
        }
    }

    //height = var1;
    IsModified = true;
}
int Chunk::GetHeightValue(int var1, int var2) {
    return HeightMap[var2 << 4 | var1] & 255;
}

void Chunk::RenderOpaqueAndPlants() {
    Game::e_DefaultShader.use();
    Game::e_DefaultShader.setMat4("view", Game::View);
    Game::e_DefaultShader.setMat4("projection", Game::Proj);
    glm::vec3 relativePos = glm::dvec3(ChunkX * Chunk_Width, 0, ChunkZ * Chunk_Length) - Game::player.GetPosition();
    Game::e_DefaultShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));

    //enable rendering features for opaque blocks
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back faces
    glFrontFace(GL_CW); // Clockwise instead

    glBindVertexArray(meshes.m_VAO);
    glDrawElements(GL_TRIANGLES, meshes.opaqueCount, GL_UNSIGNED_INT, 0);

    //disable rendering features for plants
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(meshes.m_VAO2);
    glDrawElements(GL_TRIANGLES, meshes.plantCount, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
}
void Chunk::RenderWater() {
    Game::e_WaterShader.use();
    Game::e_WaterShader.setMat4("view", Game::View);
    Game::e_WaterShader.setMat4("projection", Game::Proj);
    glm::vec3 relativePos = glm::dvec3(ChunkX * Chunk_Width, 0, ChunkZ * Chunk_Length) - Game::player.GetPosition();
    Game::e_WaterShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));
    Game::e_WaterShader.setFloat("Time", SDL_GetTicks() / 1000.0f);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    glBindVertexArray(meshes.m_VAO3);
    glDrawElements(GL_TRIANGLES, meshes.waterCount, GL_UNSIGNED_INT, 0);
}


ChunkMeshUpload CreateChunkMeshData(Chunk& chunk) {
    uint32_t index = 0;
    uint32_t index1 = 0;
    uint32_t index2 = 0;

    ChunkMeshUpload ret{};

    for (int x = 0; x < Chunk_Width; x++) {
        for (int y = 0; y < Chunk_Height; y++) {
            for (int z = 0; z < Chunk_Length; z++) {
                BlockType type = chunk.m_Blocks[IndexAt(x, y, z)];
                if (type == BlockType::Air)
                    continue;

                glm::u8vec3 blockPos(x, y, z);

                auto isAir = [&](int dx, int dy, int dz, BlockVisiblity visibility) -> bool { //for opaque blocks
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy, nz = z + dz;
                    if (nx < 0) { //check if outside on the X on the negative
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX - 1, chunk.ChunkZ);
                        BlockType type = c->m_Blocks[IndexAt(15, ny, nz)];
                        return type == BlockType::Air || Game::e_BlockRegistery[type].visibility != visibility;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX + 1, chunk.ChunkZ);
                        BlockType type = c->m_Blocks[IndexAt(0, ny, nz)];
                        return type == BlockType::Air || Game::e_BlockRegistery[type].visibility != visibility;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true;
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX, chunk.ChunkZ - 1);
                        BlockType type = c->m_Blocks[IndexAt(nx, ny, 15)];
                        return type == BlockType::Air || Game::e_BlockRegistery[type].visibility != visibility;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX, chunk.ChunkZ + 1);
                        BlockType type = c->m_Blocks[IndexAt(nx, ny, 0)];
                        return type == BlockType::Air || Game::e_BlockRegistery[type].visibility != visibility;
                    }

                    //if inside the chunk
                    return chunk.m_Blocks[IndexAt(nx, ny, nz)] == BlockType::Air || Game::e_BlockRegistery[chunk.m_Blocks[IndexAt(nx, ny, nz)]].visibility != visibility;
                    };
                auto isVis = [&](int dx, int dy, int dz, BlockVisiblity visibility)  -> bool {
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy, nz = z + dz;
                    if (nx < 0) { //check if outside on the X on the negative
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX - 1, chunk.ChunkZ);
                        BlockType type = c->m_Blocks[IndexAt(15, ny, nz)];
                        return Game::e_BlockRegistery[type].visibility == visibility;
                    }
                    else if (nx >= Chunk_Width) { //check if outside on the X on the positive
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX + 1, chunk.ChunkZ);
                        BlockType type = c->m_Blocks[IndexAt(0, ny, nz)];
                        return Game::e_BlockRegistery[type].visibility == visibility;
                    }
                    else if (ny < 0 || ny >= Chunk_Height) //check if outside on the Y axis
                    {
                        return true;
                    }
                    else if (nz < 0) { //check if outside on the Z on the negative
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX, chunk.ChunkZ - 1);
                        BlockType type = c->m_Blocks[IndexAt(nx, ny, 15)];
                        return Game::e_BlockRegistery[type].visibility == visibility;
                    }
                    else if (nz >= Chunk_Length) { //check if outside on the Z on the positive
                        Chunk* c = chunk.owningWorld->GetChunkProvider().ProvideChunk(chunk.ChunkX, chunk.ChunkZ + 1);
                        BlockType type = c->m_Blocks[IndexAt(nx, ny, 0)];
                        return Game::e_BlockRegistery[type].visibility == visibility;
                    }

                    //if inside the chunk
                    return  Game::e_BlockRegistery[chunk.m_Blocks[IndexAt(nx, ny, nz)]].visibility == visibility;
                    };
                
                BlockData bd = Game::e_BlockRegistery[chunk.m_Blocks[IndexAt(x, y, z)]];
                switch (Game::e_BlockRegistery[chunk.m_Blocks[IndexAt(x, y, z)]].visibility) {
                case BlockVisiblity::Opaque:
                    if (isAir(0, 0, -1, Opaque)) AddFace(blockPos, Face::Back, bd.uv.Back, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, 0, 1, Opaque)) AddFace(blockPos, Face::Front, bd.uv.Front, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(-1, 0, 0, Opaque)) AddFace(blockPos, Face::Left, bd.uv.Left, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(1, 0, 0, Opaque)) AddFace(blockPos, Face::Right, bd.uv.Right, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, 1, 0, Opaque)) AddFace(blockPos, Face::Top, bd.uv.Top, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, -1, 0, Opaque)) AddFace(blockPos, Face::Bottom, bd.uv.Bottom, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    break;
                case BlockVisiblity::Plant:
                    AddPlantFace(blockPos, bd.uv.Back, index1, ret.plantVerticies, ret.plantIndicies);
                    break;
                case BlockVisiblity::Liquid:
                    if (isAir(0, 0, -1, Liquid)) AddLiquidFace(blockPos, Face::Back, bd.uv.Back, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, 0, 1, Liquid)) AddLiquidFace(blockPos, Face::Front, bd.uv.Front, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(-1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Left, bd.uv.Left, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Right, bd.uv.Right, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, 1, 0, Liquid)) AddLiquidFace(blockPos, Face::Top, bd.uv.Top, index2, false, ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, -1, 0, Liquid)) AddLiquidFace(blockPos, Face::Bottom, bd.uv.Bottom, index2, false, ret.waterVerticies, ret.waterIndicies);
                    break;
                }
            }
        }
    }

    return ret;
}
void Chunk::ChunkUpload(ChunkMeshUpload& meshData) {
    if (meshData.opaqueVerticies.size() > 0)
    {
        // Upload to GPU
        glBindVertexArray(meshes.m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshes.m_VBO);
        glBufferData(GL_ARRAY_BUFFER, meshData.opaqueVerticies.size() * sizeof(Vertex), meshData.opaqueVerticies.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.opaqueIndicies.size() * sizeof(uint32_t), meshData.opaqueIndicies.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasOpaque = true;
        meshes.opaqueCount = meshData.opaqueIndicies.size();
    }
    else HasOpaque = false;



    //Set Up plant mesh
    if (meshData.plantVerticies.size() > 0)
    {
        // Upload to GPU
        glBindVertexArray(meshes.m_VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, meshes.m_VBO2);
        glBufferData(GL_ARRAY_BUFFER, meshData.plantVerticies.size() * sizeof(Vertex), meshData.plantVerticies.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.m_EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.plantIndicies.size() * sizeof(uint32_t), meshData.plantIndicies.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasPlant = true;
        meshes.plantCount = meshData.plantIndicies.size();
    }
    else HasPlant = false;



    //Set Up liquid mesh
    if (meshData.waterVerticies.size() > 0) {
        glBindVertexArray(meshes.m_VAO3);
        glBindBuffer(GL_ARRAY_BUFFER, meshes.m_VBO3);
        glBufferData(GL_ARRAY_BUFFER, meshData.waterVerticies.size() * sizeof(Vertex), meshData.waterVerticies.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes.m_EBO3);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.waterIndicies.size() * sizeof(uint32_t), meshData.waterIndicies.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasWater = true;
        meshes.waterCount = meshData.waterIndicies.size();
    }
    else HasWater = false;
}
void Chunk::UpdateMesh() {
    ChunkMeshUpload meshData = CreateChunkMeshData(*this);

    ChunkUpload(meshData);
}

int GetLODSize(uint8_t LOD) {
    return 1 << LOD;
}
void SuperChunk::Render() {
    RenderOpaque();
    RenderWater();
}
void SuperChunk::RenderOpaque() {
    if (HasOpaque) {
        Game::e_DefaultShader.use();
        Game::e_DefaultShader.setMat4("view", Game::View);
        Game::e_DefaultShader.setMat4("projection", Game::Proj);
        uint8_t LODFactor = GetLODSize(LOD);
        glm::vec3 relativePos = glm::dvec3(Pos.x * Chunk_Width * LODFactor, 0, Pos.y * Chunk_Length * LODFactor) - Game::player.GetPosition();
        glm::vec3 scale = glm::vec3(LODFactor);
        Game::e_DefaultShader.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0), relativePos), scale));

        //enable rendering features for opaque blocks
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Cull back faces
        glFrontFace(GL_CW); // Clockwise instead

        glBindVertexArray(mesh.m_VAO);
        glDrawElements(GL_TRIANGLES, mesh.opaqueCount, GL_UNSIGNED_INT, 0);
    }
}
void SuperChunk::RenderWater() {
    if (HasWater) {
        Game::e_WaterShader.use();
        Game::e_WaterShader.setMat4("view", Game::View);
        Game::e_WaterShader.setMat4("projection", Game::Proj);
        uint8_t LODFactor = GetLODSize(LOD);
        glm::vec3 relativePos = glm::dvec3(Pos.x * Chunk_Width * LODFactor, 0, Pos.y * Chunk_Length * LODFactor) - Game::player.GetPosition();
        glm::vec3 scale = glm::vec3(LODFactor);
        Game::e_WaterShader.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0), relativePos), scale));
        Game::e_WaterShader.setFloat("Time", SDL_GetTicks() / 1000.0f);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);

        glBindVertexArray(mesh.m_VAO3);
        glDrawElements(GL_TRIANGLES, mesh.waterCount, GL_UNSIGNED_INT, 0);
    }
}

SuperChunkMeshUpload CreateSuperChunkMeshData(BlockType* voxelData, uint8_t LOD) {
    uint32_t index = 0;
    uint32_t index2 = 0;
    uint8_t LODFactor = 1 << LOD;

    SuperChunkMeshUpload ret;

    for (int x = 0; x < Chunk_Width; x++) {
        for (int y = 0; y < Chunk_Height; y += 2) {
            for (int z = 0; z < Chunk_Length; z++) {
                BlockType type = voxelData[IndexAt(x, y, z)];
                if (type == BlockType::Air)
                    continue;

                glm::uvec3 blockPos(x, y / LODFactor, z);

                auto isAir = [&](int dx, int dy, int dz, BlockVisiblity visibility) -> bool { //for opaque blocks
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy * LODFactor, nz = z + dz;
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
                    return voxelData[IndexAt(nx, ny, nz)] == BlockType::Air || Game::e_BlockRegistery[voxelData[IndexAt(nx, ny, nz)]].visibility != visibility;
                    };
                auto isVis = [&](int dx, int dy, int dz, BlockVisiblity visibility)  -> bool {
                    Block* block;
                    int nx, ny, nz; //these are the full coords in chunk space not just the offset
                    nx = x + dx, ny = y + dy * LODFactor, nz = z + dz;
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
                    return  Game::e_BlockRegistery[voxelData[IndexAt(nx, ny, nz)]].visibility == visibility;
                    };

                BlockData bd = Game::e_BlockRegistery[voxelData[IndexAt(x, y, z)]];
                switch (bd.visibility) {
                case BlockVisiblity::Opaque:
                    if (isAir(0, 0, -1, Opaque)) AddFace(blockPos, Face::Back, bd.uv.Back, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, 0, 1, Opaque)) AddFace(blockPos, Face::Front, bd.uv.Front, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(-1, 0, 0, Opaque)) AddFace(blockPos, Face::Left, bd.uv.Left, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(1, 0, 0, Opaque)) AddFace(blockPos, Face::Right, bd.uv.Right, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, 1, 0, Opaque)) AddFace(blockPos, Face::Top, bd.uv.Top, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    if (isAir(0, -1, 0, Opaque)) AddFace(blockPos, Face::Bottom, bd.uv.Bottom, index, ret.opaqueVerticies, ret.opaqueIndicies);
                    break;
                case BlockVisiblity::Liquid:
                    if (isAir(0, 0, -1, Liquid)) AddLiquidFace(blockPos, Face::Back, bd.uv.Back, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, 0, 1, Liquid)) AddLiquidFace(blockPos, Face::Front, bd.uv.Front, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(-1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Left, bd.uv.Left, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(1, 0, 0, Liquid)) AddLiquidFace(blockPos, Face::Right, bd.uv.Right, index2, isVis(0, 1, 0, Liquid), ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, 1, 0, Liquid)) AddLiquidFace(blockPos, Face::Top, bd.uv.Top, index2, false, ret.waterVerticies, ret.waterIndicies);
                    if (isAir(0, -1, 0, Liquid)) AddLiquidFace(blockPos, Face::Bottom, bd.uv.Bottom, index2, false, ret.waterVerticies, ret.waterIndicies);
                    break;
                }
            }
        }
    }
    return ret;
}
void SuperChunk::ChunkUpload(SuperChunkMeshUpload& meshData) {
    if (meshData.opaqueVerticies.size() > 0)
    {
        glBindVertexArray(mesh.m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBO);
        glBufferData(GL_ARRAY_BUFFER, meshData.opaqueVerticies.size() * sizeof(Vertex), meshData.opaqueVerticies.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.opaqueIndicies.size() * sizeof(uint32_t), meshData.opaqueIndicies.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasOpaque = true;
        mesh.opaqueCount = meshData.opaqueIndicies.size();
    }
    else HasOpaque = false;

    if (meshData.waterVerticies.size() > 0) {
        glBindVertexArray(mesh.m_VAO3);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBO3);
        glBufferData(GL_ARRAY_BUFFER, meshData.waterVerticies.size() * sizeof(Vertex), meshData.waterVerticies.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_EBO3);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.waterIndicies.size() * sizeof(uint32_t), meshData.waterIndicies.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        HasWater = true;
        mesh.waterCount = meshData.waterIndicies.size();
    }
    else HasWater = false;
}

void SuperChunk::CreateMeshObjects() {
    glGenVertexArrays(1, &mesh.m_VAO);
    glGenBuffers(1, &mesh.m_VBO);
    glGenBuffers(1, &mesh.m_EBO);

    glGenVertexArrays(1, &mesh.m_VAO3);
    glGenBuffers(1, &mesh.m_VBO3);
    glGenBuffers(1, &mesh.m_EBO3);
}
void SuperChunk::DeleteMeshObjects() {
    glDeleteVertexArrays(1, &mesh.m_VAO);
    glDeleteBuffers(1, &mesh.m_VBO);
    glDeleteBuffers(1, &mesh.m_EBO);

    glDeleteVertexArrays(1, &mesh.m_VAO3);
    glDeleteBuffers(1, &mesh.m_VBO3);
    glDeleteBuffers(1, &mesh.m_EBO3);
}




//helper functions
uint16_t convertPos(glm::uvec3 pos) {
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
void AddFace(glm::uvec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies) {
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
void AddPlantFace(glm::uvec3 pos, uint8_t faceIndexOffset, uint32_t& indexOffset, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies) {
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
void AddLiquidFace(glm::uvec3 pos, Face face, uint8_t faceIndexOffset, uint32_t& indexOffset, bool IsMiddle, std::vector<Vertex>& verticies, std::vector<uint32_t>& indicies) {
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