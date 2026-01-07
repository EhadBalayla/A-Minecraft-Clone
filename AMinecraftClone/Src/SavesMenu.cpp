#include "SavesMenu.h"
#include "Game.h"

void CancelButtonCallback() {
	if (!Game::savesMenu.IsDeleting) Game::MainMenuState = MainMenuMenu::Title;
	else {
		Game::savesMenu.IsDeleting = false;
		Game::savesMenu.titleText.setText("Select world");
	}
}

void DeleteButtonCallback() {
	if (!Game::savesMenu.IsDeleting) {
		Game::savesMenu.IsDeleting = true;
		Game::savesMenu.titleText.setText("Delete world");
	}
}

void WorldButtonClick() {
	Game::ChangeState(GameState::InGame);
}

void ChangeWorldType() {
	if (Game::m_ChosenTerrain >= 2) {
		Game::m_ChosenTerrain = 0;
		Game::savesMenu.WorldType.SetText("World Type: Infdev-20100227");
	}
	else {
		Game::m_ChosenTerrain++;
		if(Game::m_ChosenTerrain == 1) Game::savesMenu.WorldType.SetText("World Type: Infdev-20100327");
		else if(Game::m_ChosenTerrain == 2) Game::savesMenu.WorldType.SetText("World Type: Infdev-20100630");
	}
}

SavesMenu::SavesMenu() {
	titleText.position = glm::vec2(560, 72);
	titleText.setText("Select world");

	CancelButton.position = glm::vec2(640, 655);
	CancelButton.scale = glm::vec2(600, 60);
	CancelButton.SetText("Cancel");
	CancelButton.Callback = CancelButtonCallback;

	DeleteWorldButton.position = glm::vec2(640, 545);
	DeleteWorldButton.scale = glm::vec2(600, 60);
	DeleteWorldButton.SetText("Delete World...");
	DeleteWorldButton.Callback = DeleteButtonCallback;

	int index = 0;
	for (auto& n : WorldButtons) {
		n.position = glm::vec2(640, 150 + index * 72);
		n.scale = glm::vec2(600, 60);
		n.SetText("- empty -");
		n.Callback = WorldButtonClick;
		index++;
	}

	WorldType.position = glm::vec2(1025, 40);
	WorldType.scale = glm::vec2(490, 60);
	WorldType.SetText("World Type: Infdev-20100227");
	WorldType.Callback = ChangeWorldType;
}

void SavesMenu::RenderScreen() {
	titleText.Render();
	CancelButton.Render();
	if (!IsDeleting) DeleteWorldButton.Render();
	for (auto& n : WorldButtons) {
		n.Render();
	}
	WorldType.Render();
}
void SavesMenu::UpdateScreen() {
	CancelButton.Update();
	if(!IsDeleting) DeleteWorldButton.Update();
	for (auto& n : WorldButtons) {
		n.Update();
	}
	WorldType.Update();
}