#include "PlayerInventory.h"
#include "Game.h"

PlayerInventory::PlayerInventory() {
	InventoryIMG.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.6875, 0.0), glm::vec2(0.6875, 0.6484375), glm::vec2(0.0, 0.6484375) };
	InventoryIMG.tex = &Game::inventoryTex;
	InventoryIMG.position = glm::vec2(640, 360);
	InventoryIMG.scale = glm::vec2(530.120, 500);
}

void PlayerInventory::RenderScreen() {
	InventoryIMG.Render();
}
void PlayerInventory::UpdateScreen() {

}