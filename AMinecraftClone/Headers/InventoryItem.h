#pragma once
#include "Item.h"

class InventoryItem
{
public:
	ItemType m_Item;
	int m_Amount = 0;

	InventoryItem() = default; //a default constructor that returns no item
	
	void addAmount(int amount);
	void removeAmount(int amount);
};

