#pragma once
#include "AABB.h"

struct BlockUV {
	uint8_t Top = 0;
	uint8_t Bottom = 0;
	uint8_t Left = 0;
	uint8_t Right = 0;
	uint8_t Front = 0;
	uint8_t Back = 0;
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

enum BlockType : uint8_t { //block ids
	Air, 
	Grass, 
	Stone, 
	Dirt, 
	Sand,
	Gravel,
	Cobblestone,
	MossyCobblestone,
	WoodenPlanks,
	Wood,
	Bedrock,
	Bricks,
	TNT,
	CoalOre,
	IronOre,
	GoldOre,
	DiamondOre,
	Glass,
	MobSpawner,
	Obsidian,
	YellowFlower,
	RedFlower,
	BrownMushroom,
	RedMushroom,
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
	BlockData getData() { return data; };
	void setType(BlockType newType);

	void BreakBlock();
	glm::ivec3 getWorldPosition();
};

