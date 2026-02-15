#include "InventoryItem.h"
#include "Game.h"

void InventoryItem::addAmount(int amount) {
	ItemData id = Game::e_ItemRegistery[m_Item];
	if (m_Amount + amount > id.StackAmount)
		m_Amount = id.StackAmount;
	else
		m_Amount += amount;
}

void InventoryItem::removeAmount(int amount) {
	if (m_Amount - amount < 0)
		m_Amount = 0;
	else
		m_Amount -= amount;
}