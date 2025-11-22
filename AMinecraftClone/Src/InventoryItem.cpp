#include "InventoryItem.h"

void InventoryItem::addAmount(uint8_t amount) {
	if (m_Amount + amount > m_Item.getData().StackAmount)
		m_Amount = m_Item.getData().StackAmount;
	else
		m_Amount += amount;
}

void InventoryItem::removeAmount(uint8_t amount) {
	if (m_Amount - amount < 0)
		m_Amount = 0;
	else
		m_Amount -= amount;
}