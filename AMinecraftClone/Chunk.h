#pragma once
#include "Block.h"
#include <glm/glm.hpp>
#include <vector>

constexpr int Chunk_Width = 16;
constexpr int Chunk_Height = 128;
constexpr int Chunk_Length = 16;

constexpr int VOXEL_ARRAY_SIZE = Chunk_Width * Chunk_Height * Chunk_Length;
constexpr int HEIGHT_MAP_SIZE = Chunk_Width * Chunk_Length;

inline int IndexAt(int x, int y, int z) {
	return x + z * Chunk_Width + y * Chunk_Width * Chunk_Length;
}
inline int HeightIndexAt(int x, int z) {
	return x + z * Chunk_Width;
}

struct Vertex {
	uint16_t pos; //4 bits for chunk X, 4 bits for chunk Z, 7 bits for chunk Y
	uint8_t texIndex; //since the texture atlas is 16x16
	uint8_t extras; //2 bits for which corner on the image in the atlas it is, 3 bits for which corner of a block it is
};

struct ChunkMeshData {
	std::vector<Vertex> opaqueVerticies;
	std::vector<uint32_t> opaqueIndicies;

	std::vector<Vertex> plantVerticies;
	std::vector<uint32_t> plantIndicies;

	std::vector<Vertex> waterVerticies;
	std::vector<uint32_t> waterIndicies;
};

struct ChunkMesh {
	unsigned int m_VAO, m_VBO, m_EBO, opaqueCount = 0;
	unsigned int m_VAO2, m_VBO2, m_EBO2, plantCount = 0;
	unsigned int m_VAO3, m_VBO3, m_EBO3, waterCount = 0;
};

enum Face {
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back
};

class WorldManager;
class Chunk
{
public:
	BlockType m_Blocks[VOXEL_ARRAY_SIZE];
	int HeightMap[HEIGHT_MAP_SIZE] = {};

	WorldManager* owningWorld;
	int ChunkX, ChunkZ;

	//flags
	bool IsGenerated = false;
	bool IsPopulated = false;
	bool IsModified = false;
	bool IsRenderReady = false;
	bool IsMeshed = false;

	//meshes flags
	bool HasOpaque = false;
	bool HasPlant = false;
	bool HasWater = false;

	void RenderOpaque();
	void RenderPlants();
	void RenderWater();
	void CreateChunkMeshData();
	void ChunkUpload();

	void CreateMeshObjects();
	void DeleteMeshObjects();

	void GenerateHeightMap();
	int GetHeightValue(int var1, int var2);
private:
	ChunkMeshData meshData;
	ChunkMesh meshes; //meshes for each LOD
};







//LOD chunks stuff
int GetLODSize(uint8_t LOD);
struct SuperChunkMeshData {
	std::vector<Vertex> opaqueVerticies;
	std::vector<uint32_t> opaqueIndicies;

	std::vector<Vertex> waterVerticies;
	std::vector<uint32_t> waterIndicies;
};
struct SuperChunkMesh {
	unsigned int m_VAO, m_VBO, m_EBO, opaqueCount = 0;
	unsigned int m_VAO3, m_VBO3, m_EBO3, waterCount = 0;
};

class SuperChunk {
public:
	uint8_t LOD = 1; //from 1 - 4
	BlockType m_Blocks[VOXEL_ARRAY_SIZE];

	WorldManager* owningWorld;
	int ChunkX, ChunkZ;

	//flags
	bool IsGenerated = false;
	bool IsPopulated = false;
	bool IsModified = false;
	bool IsRenderReady = false;
	bool IsMeshed = false;

	//meshes flags
	bool HasOpaque = false;
	bool HasWater = false;

	void Render();
	void CreateSuperChunkMeshData();
	void ChunkUpload();

	void ChunkUpload(SuperChunkMeshData& meshData);

	void CreateMeshObjects();
	void DeleteMeshObjects();
private:
	SuperChunkMeshData meshData;
	SuperChunkMesh mesh;

	void RenderOpaque();
	void RenderWater();
};

