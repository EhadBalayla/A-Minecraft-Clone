#include "Game.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// the screens for adding em
#include "PlayerHUDScreen.h"
#include "MainMenuScreen.h"

#include "backends/imgui_impl_sdl2.h"

Window Game::e_Window;
Shader Game::e_OpaqueShader;
Shader Game::e_PlantsShader;
Shader Game::e_WaterShader;
Shader Game::e_CloudShader;
Shader Game::e_ChunkBorderShader;
Shader Game::e_SkyColorShader;
Shader Game::e_DummyPlayersShader;
Shader Game::e_InventoryBlockShader;
Shader Game::e_ImageWidgetShader;
Shader Game::e_TextShader;
Shader Game::e_MainMenuBackgroundShader;
Texture Game::terrainAtlas;
Texture Game::guiAtlas;
Texture Game::iconsAtlas;
Texture Game::inventoryTex;
Texture Game::cloudsTex;
Texture Game::fontTex;
Texture Game::logoTex;
Texture Game::dirtTex;
PlayerInventory Game::inventoryMenu;
PlayerHUDScreen Game::hudScreen;
PauseMenu Game::pauseMenu;
MainMenuScreen Game::mainMenuScreen;
OptionsMenu Game::optionsMenu;
SavesMenu Game::savesMenu;
glm::mat4 Game::ScreenProjection;
glm::mat4 Game::Proj;
glm::mat4 Game::View;
Player Game::player;
Level* Game::level;
std::unordered_map<BlockType, BlockData> Game::e_BlockRegistery;
std::unordered_map<ItemType, ItemData> Game::e_ItemRegistery;
std::unordered_map<char, Glyph> Game::e_FontRegistery;
bool Game::IsGameRunning = true;
AudioManager Game::m_AudioManager;
bool Game::ShowChunkBorder = false;
GameState Game::state = GameState::MainMenu;
GameState Game::lastState = GameState::MainMenu;
unsigned int Game::tempVAO;
DebugRenderer Game::m_DebugRenderer;
DebugUIManager Game::m_DebugUI;
bool Game::ShowDebugMenu = true;
int Game::m_ChosenTerrain = 0;
NetworkingManager Game::m_Networking;
int Game::RenderDistance = 16;
int Game::LODCount = 5;
MainMenuMenu Game::MainMenuState = MainMenuMenu::Title;
InGameMenu Game::InGameMenuState = InGameMenu::PauseMenu;
GameState Game::GetGameState() { return state; }
bool Game::FirstClick = false;


#ifdef _WIN32

#include <Windows.h>

#endif

void Game::Init() {
#ifdef _WIN32
	FreeConsole();
#endif
	e_Window.Init();
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}


	e_OpaqueShader.loadShader("Shaders/OpaqueShader_Vert.glsl", "Shaders/OpaqueShader_Frag.glsl");
	e_PlantsShader.loadShader("Shaders/PlantsShader_Vert.glsl", "Shaders/PlantsShader_Frag.glsl");
	e_WaterShader.loadShader("Shaders/WaterShader_Vert.glsl", "Shaders/WaterShader_Frag.glsl");
	e_CloudShader.loadShader("Shaders/CloudsShader_Vert.glsl", "Shaders/CloudsShader_Frag.glsl");
	e_ChunkBorderShader.loadShader("Shaders/ChunkBorder_Vert.glsl", "Shaders/ChunkBorder_Frag.glsl");
	e_SkyColorShader.loadShader("Shaders/SkyShader_Vert.glsl", "Shaders/SkyShader_Frag.glsl");
	e_DummyPlayersShader.loadShader("Shaders/TempPlayersShader_Vert.glsl", "Shaders/TempPlayersShader_Frag.glsl");
	e_InventoryBlockShader.loadShader("Shaders/InventoryItem_Vert.glsl", "Shaders/InventoryItem_Frag.glsl");
	e_ImageWidgetShader.loadShader("Shaders/UIShader_Vert.glsl", "Shaders/UIShader_Frag.glsl");
	e_TextShader.loadShader("Shaders/TextShader_Vert.glsl", "Shaders/TextShader_Frag.glsl");
	e_MainMenuBackgroundShader.loadShader("Shaders/MainMenuBackground_Vert.glsl", "Shaders/MainMenuBackground_Frag.glsl");
	glGenVertexArrays(1, &tempVAO);


	terrainAtlas.LoadTexture("TextureAtlas.png");
	guiAtlas.LoadTexture("GUI/gui.png");
	iconsAtlas.LoadTexture("GUI/icons.png");
	inventoryTex.LoadTexture("GUI/inventory.png");
	cloudsTex.LoadTexture("SkyTextures/clouds.png");
	fontTex.LoadTexture("DefaultFont.png");
	logoTex.LoadTexture("GUI/logo.png");
	dirtTex.LoadTexture("dirt.png");


	Proj = glm::perspective(glm::radians(70.0f), 1920.0f / 1080.0f, 0.1f, 50000.0f);
	ScreenProjection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);


	m_AudioManager.Init();
	m_DebugUI.Init();
	m_Networking.Init();


	RegisterAllBlocks();
	RegisterAllItems();
	LoadFont();
}
void Game::GameLoop() {

	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	//stuff for deltatime calculations
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;
	float deltaTime = 0.0;

	SDL_Event event;

	LoadState();

	//the actualy game loop
	while (IsGameRunning) { //the game loop
		if (lastState != state) {
			UnloadState();
			state = lastState;
			LoadState();
		}

		//these 3 lines are for updating DeltaTime
		last = now;
		now = SDL_GetPerformanceCounter();
		deltaTime = ((now - last) * 1000) / SDL_GetPerformanceFrequency();

		//simply put set the clear color
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (state) {
		case GameState::MainMenu:
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					CloseGame(); //quit
				}
				ImGui_ImplSDL2_ProcessEvent(&event);
			}

			//render the background of the main menu
			dirtTex.bind();
			e_MainMenuBackgroundShader.use();
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glBindVertexArray(tempVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
	
			switch (MainMenuState) {
			case MainMenuMenu::Title:
				//render and update the main menu screen
				mainMenuScreen.UpdateScreen();
				mainMenuScreen.RenderScreen();
				break;
			case MainMenuMenu::SinglePlayer:
				//render and update the world selection screen
				savesMenu.UpdateScreen();
				savesMenu.RenderScreen();
				break;
			case MainMenuMenu::MultiPlayer:

				break;
			case MainMenuMenu::Options:
				//render and update the options menu inside the main menu (not the in game version)
				optionsMenu.UpdateScreen();
				optionsMenu.RenderScreen();
				break;
			}

			if (ShowDebugMenu) m_DebugUI.Render2();

			break;
		case GameState::InGame:
			while (SDL_PollEvent(&event)) {
				player.ProcessInput(event);
				if (event.type == SDL_QUIT) {
					CloseGame(); //quit
				}
				ImGui_ImplSDL2_ProcessEvent(&event);
			}

			player.Update(deltaTime);
			level->LevelUpdate(deltaTime);

			//set the view matrix to the current camera's view
			View = player.getViewMatrix();

			level->RenderLevel();
			m_Networking.Render();

			m_DebugRenderer.DrawChunkBoundaries();

			hudScreen.RenderScreen();
			if (player.IsPaused) {
				switch(InGameMenuState) {
				case InGameMenu::PauseMenu:
					pauseMenu.UpdateScreen();
					pauseMenu.RenderScreen();
					break;
				case InGameMenu::Options:
					optionsMenu.UpdateScreen();
					optionsMenu.RenderScreen();
					break;
				}
			}
			if (player.IsInventory) {
				inventoryMenu.RenderScreen();
			}

			if (ShowDebugMenu) m_DebugUI.Render(deltaTime);
			break;
		}

		//swap buffers
		SDL_GL_SwapWindow(e_Window.m_Window);
	}

	UnloadState();
}
// terminates all engines for the game when the game closes and unloads every cached object
void Game::Terminate() {
	m_Networking.Terminate();

	m_DebugUI.Terminate();

	m_AudioManager.Terminate();
	e_Window.Termintate();
}

void Game::RegisterAllBlocks() { //register all block types in the hash map so they can be replicated for changing a block's type with custom parameters
	e_BlockRegistery[Air] = { { 0, 0, 0, 0, 0, 0 }, BlockVisiblity::Opaque }; //registers the AirBlock... not that it matters since Air has no physical appearance and is disregarded in the creation of a chunk but still
	e_BlockRegistery[Grass] = { { 0, 2, 3, 3, 3, 3 }, BlockVisiblity::Opaque }; //register the grass block
	e_BlockRegistery[Stone] = { { 1, 1, 1, 1, 1, 1 }, BlockVisiblity::Opaque }; //register the stone block
	e_BlockRegistery[Dirt] = { { 2, 2, 2, 2, 2, 2 }, BlockVisiblity::Opaque }; //register the dirt block
	e_BlockRegistery[Sand] = { { 18, 18, 18, 18, 18, 18 }, BlockVisiblity::Opaque }; //register the sand block
	e_BlockRegistery[Gravel] = { { 19, 19, 19, 19, 19, 19 }, BlockVisiblity::Opaque }; //register the gravel block
	e_BlockRegistery[Cobblestone] = { { 16, 16, 16, 16, 16, 16 }, BlockVisiblity::Opaque }; //register the cobblestone block
	e_BlockRegistery[MossyCobblestone] = { { 36, 36, 36, 36, 36, 36 }, BlockVisiblity::Opaque }; //register the mossy cobblestone block
	e_BlockRegistery[WoodenPlanks] = { { 4, 4, 4, 4, 4, 4 }, BlockVisiblity::Opaque }; //register the wooden planks block
	e_BlockRegistery[Wood] = { { 21, 21, 20, 20, 20, 20 }, BlockVisiblity::Opaque }; //register the wood block
	e_BlockRegistery[Bedrock] = { { 17, 17, 17, 17, 17, 17 }, BlockVisiblity::Opaque }; //register the bedrock block
	e_BlockRegistery[Bricks] = { { 7, 7, 7, 7, 7, 7 }, BlockVisiblity::Opaque }; //register the bricks block
	e_BlockRegistery[TNT] = { { 9, 10, 8, 8, 8, 8 }, BlockVisiblity::Opaque };//register the TNT block
	e_BlockRegistery[CoalOre] = { { 34, 34, 34, 34, 34, 34 }, BlockVisiblity::Opaque }; //register the coal ore block
	e_BlockRegistery[IronOre] = { { 33, 33, 33, 33, 33, 33 }, BlockVisiblity::Opaque }; //register the iron ore block
	e_BlockRegistery[GoldOre] = { { 32, 32, 32, 32, 32, 32 }, BlockVisiblity::Opaque }; //register the gold ore block
	e_BlockRegistery[DiamondOre] = { { 50, 50, 50, 50, 50, 50 }, BlockVisiblity::Opaque }; //register the diamond ore block
	e_BlockRegistery[Glass] = { { 49, 49, 49, 49, 49, 49 }, BlockVisiblity::Transparent }; //register the glass block
	e_BlockRegistery[MobSpawner] = { { 65, 65, 65, 65, 65, 65 }, BlockVisiblity::Opaque }; //register the glass block
	e_BlockRegistery[Obsidian] = { { 37, 37, 37, 37, 37, 37 }, BlockVisiblity::Opaque }; //register the obsidian block
	e_BlockRegistery[YellowFlower] = { { 13, 13, 13, 13, 13, 13 }, BlockVisiblity::Plant }; //register the yellow flower block
	e_BlockRegistery[RedFlower] = { { 12, 12, 12, 12, 12, 12 }, BlockVisiblity::Plant }; //register the red flower block
	e_BlockRegistery[BrownMushroom] = { { 29, 29, 29, 29, 29, 29 }, BlockVisiblity::Plant }; //register the brown mushroom block
	e_BlockRegistery[RedMushroom] = { { 28, 28, 28, 28, 28, 28 }, BlockVisiblity::Plant }; //register the red flower block
	e_BlockRegistery[WaterStill] = { { 205, 205, 205, 205, 205, 205 }, BlockVisiblity::Liquid }; //register the still water block
	e_BlockRegistery[Leaves] = { {52, 52, 52, 52, 52, 52}, BlockVisiblity::Transparent }; //register the leaves block
}

void Game::RegisterAllItems() { //register all item types in the hash map so they can be replicated for changing a block's type with custom parameters
	e_ItemRegistery[NoItem] = { "No Item", ItemUsageType::PlaceableBlock, 64, BlockType::Air};
	e_ItemRegistery[GrassBlock] = { "Grass Block", ItemUsageType::PlaceableBlock, 64, BlockType::Grass }; //register the grass block item
	e_ItemRegistery[CobblestoneBlock] = { "Cobblestone Block", ItemUsageType::PlaceableBlock, 64, BlockType::Cobblestone }; //register the grass block item
	e_ItemRegistery[DiamondBlock] = { "Diamond Ore Block", ItemUsageType::PlaceableBlock, 64, BlockType::DiamondOre }; //register the grass block item
	e_ItemRegistery[BrickBlock] = { "Brick Block", ItemUsageType::PlaceableBlock, 64, BlockType::Bricks }; //register the grass block item
	e_ItemRegistery[WoodBlock] = { "Wood Block", ItemUsageType::PlaceableBlock, 64, BlockType::Wood }; //register the grass block item
	e_ItemRegistery[WoodPlankBlock] = { "Wooden Plank Block", ItemUsageType::PlaceableBlock, 64, BlockType::WoodenPlanks }; //register the grass block item
	e_ItemRegistery[BedrockBlock] = { "Bedrock Block", ItemUsageType::PlaceableBlock, 64, BlockType::Bedrock }; //register the grass block item
	e_ItemRegistery[ObsidianBlock] = { "Cobblestone Block", ItemUsageType::PlaceableBlock, 64, BlockType::Obsidian }; //register the grass block item
	e_ItemRegistery[StoneBlock] = { "Cobblestone Block", ItemUsageType::PlaceableBlock, 64, BlockType::Stone }; //register the grass block item
}

#include "stbi_image.h"

void Game::LoadFont() {
	std::string lookup = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~\u2302\u00c7\u00fc\u00e9\u00e2\u00e4\u00e0\u00e5\u00e7\u00ea\u00eb\u00e8\u00ef\u00ee\u00ec\u00c4\u00c5\u00c9\u00e6\u00c6\u00f4\u00f6\u00f2\u00fb\u00f9\u00ff\u00d6\u00dc\u00f8\u00a3\u00d8\u00d7\u0192\u00e1\u00ed\u00f3\u00fa\u00f1\u00d1\u00aa\u00ba\u00bf\u00ae\u00ac\u00bd\u00bc\u00a1\u00ab\u00bb";

	int width, height, channels;
	unsigned char* pixels = stbi_load("DefaultFont.png", &width, &height, &channels, 4);

	for (int i = 0; i < lookup.size(); i++) {
		int CellX = (i + 32) % 16;
		int CellY = (i + 32) / 16;
		glm::vec2 uv0 = glm::vec2((CellX * 8.0f) / static_cast<float>(width), (CellY * 8.0f) / static_cast<float>(height));
		glm::vec2 uv1 = glm::vec2((CellX * 8.0f + 7.99f) / static_cast<float>(width), (CellY * 8.0f + 7.99f) / static_cast<float>(height));

		int char_width = 0;
		if (lookup[i] == ' ') char_width = 4;
		else for (int x = 7; x >= 0; --x) {
			int rightmost = -1;
			for (int x = 7; x >= 0; --x) {
				bool emptyColumn = true;
				for (int y = 0; y < 8; ++y) {
					int pixel = pixels[((CellY * 8 + y) * width + (CellX * 8 + x)) * 4 + 3];
					if (pixel > 0) emptyColumn = false;
				}
				if (!emptyColumn) {
					rightmost = x;
					break;
				}
			}
			if (rightmost == -1) rightmost = 0;
			char_width = rightmost + 2;
		}

		e_FontRegistery[lookup[i]] = { char_width, uv0, uv1 };
	}

	stbi_image_free(pixels);
}

void Game::CloseGame() {
	IsGameRunning = false;
}


void Game::ChangeState(GameState newState) {
	lastState = newState;
}
void Game::UnloadState() {
	switch (state) {
	case GameState::MainMenu:
		UnloadMainMenuState();
		break;
	case GameState::InGame:
		UnloadGameState();
		break;
	}
}
void Game::LoadState() {
	switch (state) {
	case GameState::MainMenu:
		LoadMainMenuState();
		break;
	case GameState::InGame:
		LoadGameState();
		break;
	}
}



void Game::LoadMainMenuState() {
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
}
void Game::UnloadMainMenuState() {

}


void Game::LoadGameState() {
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	level = new Level();
	level->GetWorld().ChunksStart(0, 0);
	player.SetPosition(10, 100, 10);
	player.AddStarterItems();
}
void Game::UnloadGameState() {
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	delete level;
	player.IsPaused = false;
}
