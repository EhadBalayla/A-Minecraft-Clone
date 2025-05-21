#include "MenuButtons.h"
#include "Game.h"

MenuButtons::MenuButtons() : Button(
	{ glm::vec2(0.0, 0.1796875), glm::vec2(0.78125, 0.1796875), glm::vec2(0.78125, 0.2578125), glm::vec2(0.0, 0.2578125) }, //button off UV
	{ glm::vec2(0.0, 0.2578125), glm::vec2(0.78125, 0.2578125), glm::vec2(0.78125, 0.3359375), glm::vec2(0.0, 0.3359375) }, //button normal UV
	{ glm::vec2(0.0, 0.3359375), glm::vec2(0.78125, 0.3359375), glm::vec2(0.78125, 0.4140625), glm::vec2(0.0, 0.4140625) } //button hovered UV
) {
	AssignTexture(Game::e_LoadedTextures[1]);
}