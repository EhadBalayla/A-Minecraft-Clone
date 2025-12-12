#include "PlayerHUDScreen.h"
#include "Game.h"
#include "TextDisplayer.h"
#include "ItemDisplayer.h"

PlayerHUDScreen::PlayerHUDScreen() {
	Hotbar.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.7109375, 0.0), glm::vec2(0.7109375, 0.0859375), glm::vec2(0.0, 0.0859375) };
	Hotbar.tex = &Game::guiAtlas;
	Hotbar.position = glm::vec2(640, 689.7805);
	Hotbar.scale = glm::vec2(500, 60.439);

	for (int i = 0; i < 9; i++) {


		//items[i].position = glm::vec2(640, 689.7805);
		items[i].item = &Game::player.m_PlayerItems[i];
		items[i].position = glm::vec2(420.2195 + 55.0 * (float)i, 689.7805);
		items[i].scale = glm::vec2(40.0f, 40.0f);
	}

	Reticle.uv = { glm::vec2(0.0, 0.0), glm::vec2(0.05859375, 0.0), glm::vec2(0.05859375, 0.05859375), glm::vec2(0.0, 0.05859375) };
	Reticle.tex = &Game::iconsAtlas;
	Reticle.position = glm::vec2(640, 360);
	Reticle.scale = glm::vec2(50, 50);

	Selection.uv = { glm::vec2(0.0, 0.0859375), glm::vec2(0.09375, 0.0859375), glm::vec2(0.09375, 0.1796875), glm::vec2(0.0, 0.1796875) };
	Selection.tex = &Game::guiAtlas;
	Selection.position = glm::vec2(420.2195, 689.7805);
	Selection.scale = glm::vec2(60.439, 60.439);

	//Text.setText("Minecraft Infdev");
	//Text.position = glm::vec2(15.0f, 20.0f);
	//Text.scale = glm::vec2(15.0f, 15.0f);
	//Text.scale = glm::vec2(15.0f, 15.0f);
}


void PlayerHUDScreen::RenderScreen() {
	Hotbar.Render();
	for (auto& i : items) i.Render();
	Reticle.Render();
	Selection.Render();
	Text.Render();
}
void PlayerHUDScreen::UpdateScreen() {

}