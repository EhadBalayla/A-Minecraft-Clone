#include "PlayerHUDScreen.h"
#include "Game.h"
#include "TextDisplayer.h"
#include "ItemDisplayer.h"

PlayerHUDScreen::PlayerHUDScreen() {
	Hotbar.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.7109375, 0.0), glm::vec2(0.7109375, 0.0859375), glm::vec2(0.0, 0.0859375) };
	Hotbar.tex = &Game::guiAtlas;
	Hotbar.position = glm::vec2(0.0, -66.4829 / 2.0);
	Hotbar.scale = glm::vec2(550, 66.4829);
	Hotbar.projMtx = &Game::ScrMidBottomProj;

	for (int i = 0; i < 9; i++) {
		items[i].item = &Game::player.m_PlayerItems[i];
		items[i].position = glm::vec2(-241.75855 + 60.5 * (float)i, -30.2195);
		items[i].scale = glm::vec2(47.0f, 47.0f);
		items[i].projMtx = &Game::ScrMidBottomProj;
		items[i].countText.projMtx = &Game::ScrMidBottomProj;
	}

	Reticle.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.05859375, 0.0), glm::vec2(0.05859375, 0.05859375), glm::vec2(0.0, 0.05859375) };
	Reticle.tex = &Game::iconsAtlas;
	Reticle.scale = glm::vec2(50, 50);
	Reticle.projMtx = &Game::ScrMidProj;

	Selection.uv = { glm::vec2(0.0, 0.0859375), glm::vec2(0.09375, 0.0859375), glm::vec2(0.09375, 0.1796875), glm::vec2(0.0, 0.1796875) };
	Selection.tex = &Game::guiAtlas;
	Selection.position = glm::vec2(-241.75855, -30.2195);
	Selection.scale = glm::vec2(66.4829, 66.4829);
	Selection.projMtx = &Game::ScrMidBottomProj;

	Text.setText("Minecraft Infdev");
	Text.position = glm::vec2(18.0f, 18.0f);
	Text.scale = glm::vec2(15.0f, 15.0f);
	Text.projMtx = &Game::ScreenProjection;
}


void PlayerHUDScreen::RenderScreen() {
	Hotbar.Render();
	Selection.Render();
	for (auto& i : items) i.Render();
	Reticle.Render();
	Text.Render();
}
void PlayerHUDScreen::UpdateScreen() {

}