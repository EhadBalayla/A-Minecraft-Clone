#pragma once
#include "Block.h"
#include <string>

enum ItemUsageType {
	PlaceableBlock,
	Pickaxe,
	Axe,
	Shovel
};

struct ItemData {
	// all the basic data for an item
	std::string name = "No Item"; //name for the item
	ItemUsageType type = ItemUsageType::PlaceableBlock; //wether the item in quesion is of type PlaceableBlock or Melee weapon or more etc...
	int StackAmount = 64; //the amount of a stack for an item


	//advanced data for certain item types
	BlockType blockID = BlockType::Air; // the type of block if the item is a placeable block
};

enum ItemType //basically ItemIDs
{
	NoItem,
	GrassBlock,
	StoneBlock,
	DirtBlock,
	SandBlock,
	GravelBlock,
	CobblestoneBlock,
	MossyCobblestoneBlock,
	WoodPlankBlock,
	WoodBlock,
	BedrockBlock,
	BrickBlock,
	TNTBlock,
	CoalOreBlock,
	IronOreBlock,
	GoldOreBlock,
	DiamondOreBlock,
	GlassBlock,
	MobSpawnerBlock,
	ObsidianBlock,
	YellowFlowerBlock,
	RedFlowerBlock,
	LeavesBlock,
};

