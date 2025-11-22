#include "PlayerInventory.h"
#include "Game.h"

PlayerInventory::PlayerInventory() {
	// the inventory sprite
	UIUVQuad inventoryUV = { glm::vec2(0.0, 0.0), glm::vec2(0.6875, 0.0), glm::vec2(0.6875, 0.6484375), glm::vec2(0.0, 0.6484375) };
	std::unique_ptr<Widget> inventory = std::make_unique<Widget>(inventoryUV);
	inventory.get()->AssignTexture(Game::e_LoadedTextures[3]);
	inventory.get()->SetPosition(glm::vec2(640, 360));
	inventory.get()->SetScale(glm::vec2(530.120, 500));
	AddWidget(std::move(inventory));
}