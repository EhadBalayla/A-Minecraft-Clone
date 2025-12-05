#include "DebugUIManager.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include "Game.h"

void DebugUIManager::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(Game::e_Window.m_Window, Game::e_Window.m_Context);
	ImGui_ImplOpenGL3_Init();
}
void DebugUIManager::Terminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

float x = 0.0f, y = 0.0f, z = 0.0f;
void DebugUIManager::Render(float DeltaTime) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	
	ImGui::SetNextWindowSize(ImVec2(340, 350));
	ImGui::SetNextWindowPos(ImVec2(4, 33));
	ImGui::Begin("Debug Menu (f3)", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::Text(std::string("Delta Time: " + std::to_string(DeltaTime)).c_str());
	ImGui::Text(std::string("X: " + std::to_string(Game::player.GetPosition().x) + ", Y: " + std::to_string(Game::player.GetPosition().y) + ", Z: " + std::to_string(Game::player.GetPosition().z)).c_str());
	ImGui::Text(std::string("LOD0 X: " + std::to_string(Game::player.GetCurrentChunkCoords().x) + ", LOD0 Z: " + std::to_string(Game::player.GetCurrentChunkCoords().y)).c_str());
	ImGui::SetNextItemWidth(75.0f); ImGui::InputFloat("##TeleportX", &x); ImGui::SameLine(); ImGui::SetNextItemWidth(75.0f); ImGui::InputFloat("##TeleportY", &y); ImGui::SameLine(); ImGui::SetNextItemWidth(75.0f); ImGui::InputFloat("##TeleportZ", &z); ImGui::SameLine(); if (ImGui::Button("Teleport")) { Game::player.SetPosition(x, y, z); }
	ImGui::Text("Toggle LOD0 border (f) "); ImGui::SameLine(); ImGui::Checkbox("##Toggle LOD0 border: ", &Game::ShowChunkBorder);
	ImGui::Text("Toggle creative flight (c) "); ImGui::SameLine(); ImGui::Checkbox("##Toggle Creative: ", &Game::player.CreativeMode);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
const char* comboOptions[] = {
	"Infdev 2010-02-27",
	"Infdev 2010-03-27"
};
void DebugUIManager::Render2() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(340, 350));
	ImGui::SetNextWindowPos(ImVec2(4, 33));
	ImGui::Begin("Debug Menu for Main Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::TextUnformatted("Select Terrain: ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	if (ImGui::BeginCombo("##Terrain Type Select: ", comboOptions[Game::m_ChosenTerrain])) {
		for (int i = 0; i < 2; i++) { 
			if (ImGui::Selectable(comboOptions[i], Game::m_ChosenTerrain == i)) {
				Game::m_ChosenTerrain = i;
			}
		}

		ImGui::EndCombo();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}