#pragma once
#include "Block.h"
#include <iostream>
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
	CobblestoneBlock,
	StoneBlock,
	ObsidianBlock,
	DiamondBlock,
	WoodBlock,
	WoodPlankBlock,
	BrickBlock,
	BedrockBlock
};

class Item
{
public:
	void setType(ItemType newType);
	void setOnlyType(ItemType newType);
	void setData(ItemData newData);
	ItemType getType();
	ItemData getData();
	bool IsDataEqual(ItemData other);

	Item() = default;
	Item(ItemType type);


	bool operator==(const Item& other) {
		return this->IsDataEqual(other.data) && this->m_Type == other.m_Type;
	}
private:
	ItemType m_Type = NoItem;
	ItemData data;

};

