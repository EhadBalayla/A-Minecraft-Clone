#pragma once
#include "AABB.h"

struct BlockUV {
	int Top = 0;
	int Bottom = 0;
	int Left = 0;
	int Right = 0;
	int Front = 0;
	int Back = 0;
};

enum BlockType { //block ids
	Air, 
	Grass, 
	Stone, 
	Dirt, 
	Sand,
	Cobblestone,
	WoodenPlanks,
	Wood,
	Bedrock,
	Bricks,
	CoalOre,
	IronOre,
	GoldOre,
	DiamondOre,
	Glass
};

class Chunk;

class Block
{
public:
	Chunk* owningChunk;
	int BlockX, BlockY, BlockZ; //the coords of the block in chunk space

	BlockType m_Type = Air;
	BlockUV FaceUVs = {0, 0, 0, 0, 0, 0};

	static AABB blockHitbox;

	Block();
	Block(BlockType type);

	BlockType getType() { return m_Type; }
	void setType(BlockType newType);

	void BreakBlock();
	glm::ivec3 getWorldPosition();
};

