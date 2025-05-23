#pragma once
#include "Block.h"
#include <glm/glm.hpp>
#include <vector>

#include "NoiseGeneratorOctave.h"

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

	Chunk();
	~Chunk();

	Level* owningLevel;
	int ChunkX, ChunkZ;

	void GenerateChunk(NoiseGeneratorOctave* NoiseGen1, NoiseGeneratorOctave* NoiseGen2, NoiseGeneratorOctave* NoiseGen3, 
		NoiseGeneratorOctave* NoiseGen4, NoiseGeneratorOctave* NoiseGen5, NoiseGeneratorOctave* NoiseGen6,
		Random& Rand); //procedural chunk generation
	void RenderChunk();
	void GenerateMesh();

	int DistanceFromChunk(Chunk* chunk);
private:
	// a function to add a block's face to the chunk's mesh when generating a chunk mesh
	void AddFace(glm::vec3 pos, Face face, int faceIndexOffset, uint32_t& indexOffset);
	void AddPlantFace(glm::vec3 pos, int faceIndexOffset, uint32_t& indexOffset);
	void AddLiquidFace(glm::vec3 pos, Face face, int faceIndexOffset, uint32_t& indexOffset, bool IsMiddle);
	
	//3D graphics data for the chunk
	//for the opaque blocks
	std::vector<Vertex> m_Verticies;
	std::vector<unsigned int> m_Indicies; 
	unsigned int m_VAO, m_VBO, m_EBO; 

	//for the plant blocks
	std::vector<Vertex> m_Verticies2;
	std::vector<unsigned int> m_Indicies2;
	unsigned int m_VAO2, m_VBO2, m_EBO2;

	//for the liquid blocks
	std::vector<Vertex> m_Verticies3;
	std::vector<unsigned int> m_Indicies3;
	unsigned int m_VAO3, m_VBO3, m_EBO3;
};

