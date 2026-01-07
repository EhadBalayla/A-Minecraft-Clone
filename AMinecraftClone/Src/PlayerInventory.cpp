#include "PlayerInventory.h"
#include "Game.h"

PlayerInventory::PlayerInventory() {
	InventoryIMG.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.6875, 0.0), glm::vec2(0.6875, 0.6484375), glm::vec2(0.0, 0.6484375) };
	InventoryIMG.tex = &Game::inventoryTex;
	InventoryIMG.position = glm::vec2(640, 360);
	InventoryIMG.scale = glm::vec2(530.120, 500);

	for (int i = 0; i < 36; i++) {
		items[i].item = &Game::player.m_PlayerItems[i];
		items[i].scale = glm::vec2(47.0f, 47.0f);
		if (i < 9) {
			items[i].position = glm::vec2(422.0 + 54.25 * (float)i, 562.5);
		}
		else {
			items[i].position = glm::vec2(422.0 + 54.25 * (float)((i - 9) % 9), 388.0 + (54.25 * ((i - 9) / 9)));
		}
	}
}

void PlayerInventory::RenderScreen() {
	InventoryIMG.Render();
	for (auto& i : items) i.Render();
}
void PlayerInventory::UpdateScreen() {

}