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

void InventoryItem::increment() {
	ItemData id = Game::e_ItemRegistery[m_Item];
	if (m_Amount + 1 <= id.StackAmount) m_Amount++;
}
void InventoryItem::decrement() {
	if (m_Amount - 1 >= 0) {
		m_Amount--;
		if (m_Amount <= 0) m_Item = ItemType::NoItem;
	}
}

void InventoryItem::SwapSlot(InventoryItem* OtherSlot) {
	ItemType backupItem = this->m_Item;
	this->m_Item = OtherSlot->m_Item;
	OtherSlot->m_Item = backupItem;

	int backupAmount = this->m_Amount;
	this->m_Amount = OtherSlot->m_Amount;
	OtherSlot->m_Amount = backupAmount;
}

bool SlotsItemEqual(InventoryItem* slotA, InventoryItem* slotB) {
	return slotA->m_Item == slotB->m_Item;
}