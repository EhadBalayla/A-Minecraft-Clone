#include "PlayerHUDScreen.h"
#include "Game.h"

PlayerHUDScreen::PlayerHUDScreen() {
	// the hotbar
	UIUVQuad hotbarUV = { glm::vec2(0.0, 0.0), glm::vec2(0.7109375, 0.0), glm::vec2(0.7109375, 0.0859375), glm::vec2(0.0, 0.0859375)};
	std::unique_ptr<Widget> hotbar = std::make_unique<Widget>(hotbarUV);
	hotbar.get()->AssignTexture(Game::e_LoadedTextures[1]);
	hotbar.get()->SetPosition(glm::vec2(640, 689.7805));
	hotbar.get()->SetScale(glm::vec2(500, 60.439));
	AddWidget(std::move(hotbar));


	//the reticle in the center
	UIUVQuad reticleUV = { glm::vec2(0.0, 0.0), glm::vec2(0.05859375, 0.0), glm::vec2(0.05859375, 0.05859375), glm::vec2(0.0, 0.05859375) };
	std::unique_ptr<Widget> reticle = std::make_unique<Widget>(reticleUV);
	reticle.get()->AssignTexture(Game::e_LoadedTextures[2]);
	reticle.get()->SetPosition(glm::vec2(640, 360));
	reticle.get()->SetScale(glm::vec2(50, 50));
	AddWidget(std::move(reticle));


	//the hotbar selection
	UIUVQuad selectionUV = { glm::vec2(0.0, 0.0859375), glm::vec2(0.09375, 0.0859375), glm::vec2(0.09375, 0.1796875), glm::vec2(0.0, 0.1796875) };
	std::unique_ptr<Widget> selection = std::make_unique<Widget>(selectionUV);
	selection.get()->AssignTexture(Game::e_LoadedTextures[1]);
	selection.get()->SetPosition(glm::vec2(420.2195, 689.7805));
	selection.get()->SetScale(glm::vec2(60.439, 60.439));
	AddWidget(std::move(selection));
}