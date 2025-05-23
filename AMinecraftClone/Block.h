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

enum BlockVisiblity {
	Opaque,
	Plant,
	Liquid
};

struct BlockData {
	BlockUV uv = {0, 0, 0, 0, 0, 0};
	BlockVisiblity visibility = BlockVisiblity::Opaque;
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
	Glass,
	Obsidian,
	YellowFlower,
	RedFlower,
	WaterStill, //for water source blocks
	WaterFlowNorth, //for water that flows to the north
	WaterFlowEast, //for water that flows to the east
	WaterFlowSouth, //for water that flows to the south
	WaterFlowWest //for water that flows to the west
};

class Chunk;

class Block
{
public:
	Chunk* owningChunk;
	int BlockX, BlockY, BlockZ; //the coords of the block in chunk space

	BlockType m_Type = Air;
	BlockData data;

	static AABB blockHitbox;

	Block();
	Block(BlockType type);

	BlockType getType() { return m_Type; }
	void setType(BlockType newType);

	void BreakBlock();
	glm::ivec3 getWorldPosition();
};

