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
	titleText.position = glm::vec2(-80, -288);
	titleText.setText("Select world");
	titleText.projMtx = &Game::ScrMidProj;

	CancelButton.position = glm::vec2(0, 295);
	CancelButton.scale = glm::vec2(600, 60);
	CancelButton.SetText("Cancel");
	CancelButton.Callback = CancelButtonCallback;
	CancelButton.projMtx = &Game::ScrMidProj;
	CancelButton.SetTextProj(Game::ScrMidProj);

	DeleteWorldButton.position = glm::vec2(0, 185);
	DeleteWorldButton.scale = glm::vec2(600, 60);
	DeleteWorldButton.SetText("Delete World...");
	DeleteWorldButton.Callback = DeleteButtonCallback;
	DeleteWorldButton.projMtx = &Game::ScrMidProj;
	DeleteWorldButton.SetTextProj(Game::ScrMidProj);

	int index = 0;
	for (auto& n : WorldButtons) {
		n.position = glm::vec2(0, -210 + index * 72);
		n.scale = glm::vec2(600, 60);
		n.SetText("- empty -");
		n.Callback = WorldButtonClick;
		n.projMtx = &Game::ScrMidProj;
		n.SetTextProj(Game::ScrMidProj);
		index++;
	}

	WorldType.position = glm::vec2(-255, 40);
	WorldType.scale = glm::vec2(490, 60);
	WorldType.SetText("World Type: Infdev-20100227");
	WorldType.Callback = ChangeWorldType;
	WorldType.projMtx = &Game::ScrTopRightProj;
	WorldType.SetTextProj(Game::ScrTopRightProj);
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