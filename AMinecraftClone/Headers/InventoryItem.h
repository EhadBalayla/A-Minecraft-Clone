#pragma once
#include "Item.h"

class InventoryItem
{
public:
	ItemType m_Item = ItemType::NoItem;
	int m_Amount = 0;
	
	void addAmount(int amount);
	void removeAmount(int amount);

	void increment();
	void decrement();

	void SwapSlot(InventoryItem* OtherSlot);
};

bool SlotsItemEqual(InventoryItem* slotA, InventoryItem* slotB);

