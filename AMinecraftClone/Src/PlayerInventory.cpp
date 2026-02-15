#include "PlayerInventory.h"
#include "Game.h"

PlayerInventory::PlayerInventory() {
	InventoryIMG.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.6875, 0.0), glm::vec2(0.6875, 0.6484375), glm::vec2(0.0, 0.6484375) };
	InventoryIMG.tex = &Game::inventoryTex;
	InventoryIMG.scale = glm::vec2(530.120, 500);
	InventoryIMG.projMtx = &Game::ScrMidProj;

	for (int i = 0; i < 36; i++) {
		items[i].item = &Game::player.m_PlayerItems[i];
		items[i].scale = glm::vec2(47.0f, 47.0f);
		items[i].projMtx = &Game::ScrMidProj;
		items[i].SetTextProj(Game::ScrMidProj);
		if (i < 9) {
			items[i].position = glm::vec2(54.25 * (float)i - 217.0f, 202.5);
		}
		else {
			items[i].position = glm::vec2(54.25 * (float)((i - 9) % 9) - 217.0f, 28.0 + (54.25 * ((i - 9) / 9)));
		}
	}
	for (int i = 0; i < 4; i++) {
		craftingItems[i].item = &Game::player.m_CraftingItems[i];
		craftingItems[i].scale = glm::vec2(47.0f, 47.0f);
		craftingItems[i].projMtx = &Game::ScrMidProj;
		craftingItems[i].position = glm::vec2(54.25 * (float)(i % 2) + 24.0f, 54.25 * (float)(i < 2 ? 0 : 1) -148.0f);
		craftingItems[i].SetTextProj(Game::ScrMidProj);
	}

	cachedItem.item = &Game::player.m_TempItem;
	cachedItem.scale = glm::vec2(47.0f);
	cachedItem.projMtx = &Game::ScreenProjection;
	cachedItem.SetTextProj(Game::ScreenProjection);
}

void PlayerInventory::RenderScreen() {
	InventoryIMG.Render();
	for (auto& i : items) i.Render();
	for (auto& i : craftingItems) i.Render();
	cachedItem.Render();
}
void PlayerInventory::UpdateScreen() {
	for (auto& i : items) i.Update();
	for (auto& i : craftingItems) i.Update();
	
	int MouseX = 0;
	int MouseY = 0;
	auto mouseState = SDL_GetMouseState(&MouseX, &MouseY);
	cachedItem.position = glm::vec2(MouseX / Game::ScrSizeRel, MouseY / Game::ScrSizeRel);
}