#pragma once
#include "Item.h"

class InventoryItem
{
public:
	Item m_Item;
	int m_Amount = 0;

	InventoryItem() = default; //a default constructor that returns no item
	
	void addAmount(uint8_t amount);
	void removeAmount(uint8_t amount);
};

