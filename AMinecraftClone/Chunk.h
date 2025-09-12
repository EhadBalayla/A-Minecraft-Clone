#pragma once
#include "Block.h"
#include <glm/glm.hpp>
#include <vector>

constexpr int Chunk_Width = 16;
constexpr int Chunk_Height = 128;
constexpr int Chunk_Length = 16;

enum Face {
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back
};

struct Vertex {
	uint16_t pos; //4 bits for chunk X, 4 bits for chunk Z, 7 bits for chunk Y
	uint8_t texIndex; //since the texture atlas is 16x16
	uint8_t extras; //2 bits for which corner on the image in the atlas it is, 3 bits for which corner of a block it is
};

class WorldManager;

class Chunk
{
public:
	Block m_Blocks[Chunk_Width][Chunk_Height][Chunk_Length];

	Chunk();
	~Chunk();

	WorldManager* owningWorld;
	int ChunkX, ChunkZ;
	bool RenderReady = false;
	bool HasOpaque = true;
	bool HasWater = false;

	void RenderOpaqueAndPlants();
	void RenderWater();
	void GenerateMesh();

	int DistanceFromChunk(Chunk* chunk);
private:
	//3D graphics data for the chunk
	//for the opaque blocks
	unsigned int m_VAO, m_VBO, m_EBO, opaqueCount = 0;

	//for the plant blocks
	unsigned int m_VAO2, m_VBO2, m_EBO2, plantCount = 0;

	//for the liquid blocks
	unsigned int m_VAO3, m_VBO3, m_EBO3, waterCount = 0;
};

