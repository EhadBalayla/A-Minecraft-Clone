#include "Game.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// the screens for adding em
#include "PlayerHUDScreen.h"
#include "MainMenuScreen.h"

Window Game::e_Window;
Shader Game::e_OpaqueShader;
Shader Game::e_PlantsShader;
Shader Game::e_WaterShader;
Shader Game::e_CloudShader;
Shader Game::e_ChunkBorderShader;
Shader Game::e_SkyColorShader;
glm::mat4 Game::Proj;
glm::mat4 Game::View;
std::vector<Model*> Game::e_LoadedModels;
std::vector<Texture*> Game::e_LoadedTextures;
Player Game::player;
Level* Game::level;
std::unordered_map<BlockType, BlockData> Game::e_BlockRegistery;
std::unordered_map<ItemType, ItemData> Game::e_ItemRegistery;
bool Game::IsGameRunning = true;
UIManager Game::m_UIManager;
AudioManager Game::m_AudioManager;
bool Game::ShowChunkBorder = false;
GameState Game::state = GameState::MainMenu;
GameState Game::lastState = GameState::MainMenu;
unsigned int Game::tempVAO;
DebugRenderer Game::m_DebugRenderer;


void Game::Init() {
	e_Window.Init();
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	//loads in all shaders
	e_OpaqueShader.loadShader("Shaders/OpaqueShader_Vert.glsl", "Shaders/OpaqueShader_Frag.glsl");
	e_PlantsShader.loadShader("Shaders/PlantsShader_Vert.glsl", "Shaders/PlantsShader_Frag.glsl");
	e_WaterShader.loadShader("Shaders/WaterShader_Vert.glsl", "Shaders/WaterShader_Frag.glsl");
	e_CloudShader.loadShader("Shaders/CloudsShader_Vert.glsl", "Shaders/CloudsShader_Frag.glsl");
	e_ChunkBorderShader.loadShader("Shaders/ChunkBorder_Vert.glsl", "Shaders/ChunkBorder_Frag.glsl");
	e_SkyColorShader.loadShader("Shaders/SkyShader_Vert.glsl", "Shaders/SkyShader_Frag.glsl");
	glGenVertexArrays(1, &tempVAO);

	Proj = glm::mat4(1.0f);
	Proj = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 50000.0f);

	m_UIManager.Init();
	m_AudioManager.Init();

	//after finishing loading all engines and setting engine's default rendering parameters, cache in and register the necessary objects for the game
	LoadAllTextures();
	RegisterAllBlocks();
	RegisterAllItems();
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
			}
			break;
		case GameState::InGame:
			while (SDL_PollEvent(&event)) {
				player.ProcessInput(event);
				if (event.type == SDL_QUIT) {
					CloseGame(); //quit
				}
			}

			player.Update(deltaTime);
			level->LevelUpdate(deltaTime);

			//set the view matrix to the current camera's view
			View = player.getViewMatrix();

			level->RenderLevel();

			m_DebugRenderer.DrawChunkBoundaries();

			break;
		}
		m_UIManager.Update(); //after updating world information update the UI information so that by the time rendering comes it will use current data

		//render all the current UI stuff
		m_UIManager.Render();


		//swap buffers
		SDL_GL_SwapWindow(e_Window.m_Window);
	}

	UnloadState();
}
// terminates all engines for the game when the game closes and unloads every cached object
void Game::Terminate() {
	UnloadAllTextures();
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

void Game::LoadAllTextures() {// function is called at the start of the game to load everything in
	e_LoadedTextures.push_back(new Texture("TextureAtlas.png")); //the texture atlas for the blocks
	e_LoadedTextures.push_back(new Texture("GUI/gui.png")); //texture atlas for GUI stuff like player hotbar and buttons
	e_LoadedTextures.push_back(new Texture("GUI/icons.png"));
	e_LoadedTextures.push_back(new Texture("GUI/inventory.png")); //the texture for the inventory
	e_LoadedTextures.push_back(new Texture("SkyTextures/clouds.png")); //the texture atlas for the blocks
	e_LoadedTextures.push_back(new Texture("DefaultFont.png")); //the font texture
	e_LoadedTextures.push_back(new Texture("GUI/logo.png"));
}

void Game::UnloadAllTextures() {//function is called when the game closes to unload everything out
	for (auto& n : e_LoadedTextures) {
		delete n;
	}
	e_LoadedTextures.clear();
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
	auto ptr = std::make_unique<MainMenuScreen>();
	m_UIManager.AddScreen(std::move(ptr));
}
void Game::UnloadMainMenuState() {
	m_UIManager.ClearScreens();
}


void Game::LoadGameState() {
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	level = new Level();
	level->GetWorld().ChunksStart(0, 0);
	player.SetPosition(10, 100, 10);
	player.AddStarterItems();
	m_UIManager.AddScreen(std::make_unique<PlayerHUDScreen>());
}
void Game::UnloadGameState() {
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	delete level;
	m_UIManager.ClearScreens();
}
