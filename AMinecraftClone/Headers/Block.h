#pragma once
#include "AABB.h"
//a simple headerfile for enums and structs for blocks stuff

extern AABB blockHitbox;

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
	Liquid,
	Transparent,
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
	WaterFlowWest, //for water that flows to the west
	Leaves,
};

enum BlockFace {
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back
};