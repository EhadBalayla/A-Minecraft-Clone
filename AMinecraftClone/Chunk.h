#pragma once
#include "Block.h"
#include <glm/glm.hpp>
#include <vector>

#include "CombinedNoise.h"
#include "OctaveNoise.h"


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

struct UVQuad {
	glm::vec2 topLeft;
	glm::vec2 topRight;
	glm::vec2 bottomRight;
	glm::vec2 bottomLeft;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Level;

class Chunk
{
public:
	Block m_Blocks[Chunk_Width][Chunk_Height][Chunk_Length];

	//this will be the data for when a chunk is generated
	std::vector<Vertex> m_Verticies; //the coords of the vertex, the normal, and the uv
	std::vector<unsigned int> m_Indicies;
	unsigned int m_VAO, m_VBO, m_EBO;

	Chunk();
	~Chunk();

	Level* owningLevel;
	int ChunkX, ChunkZ;

	void GenerateChunk(CombinedNoise& var6, CombinedNoise& var7, OctaveNoise& var8, OctaveNoise& var54); //procedural chunk generation
	void RenderChunk();
	void GenerateMesh();

	int DistanceFromChunk(Chunk* chunk);
private:
	void AddFace(glm::vec3 pos, Face face, int faceIndexOffset, uint32_t& indexOffset); // a function to add a block's face to the chunk's mesh when generating a chunk mesh
};

