#include "Item.h"
#include "Game.h"

Item::Item(ItemType type) {
	setType(type);
}



void Item::setType(ItemType newType) {
	m_Type = newType;
	setData(Game::e_ItemRegistery[m_Type]);
}

void Item::setOnlyType(ItemType newType) {
	m_Type = newType;
}

void Item::setData(ItemData newData) {
	data = newData;
}

ItemType Item::getType() {
	return m_Type;
}

ItemData Item::getData() {
	return data;
}

bool Item::IsDataEqual(ItemData other) {
	bool areBasicDataEqual = data.name == other.name && data.type == other.type && data.StackAmount == other.StackAmount;
	bool areBlockItemDataEqual = data.blockID == other.blockID;
	return areBasicDataEqual && areBlockItemDataEqual;
}