#include "Game.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// the screens for adding em
#include "PlayerHUDScreen.h"

Window Game::e_Window;
Shader Game::e_DefaultShader;
Shader Game::e_WaterShader;
glm::mat4 Game::Proj;
glm::mat4 Game::View;
std::vector<Model*> Game::e_LoadedModels;
std::vector<Texture*> Game::e_LoadedTextures;
Player Game::player;
Level* Game::overworld;
std::unordered_map<BlockType, BlockData> Game::e_BlockRegistery;
std::unordered_map<ItemType, ItemData> Game::e_ItemRegistery;
bool Game::IsGameRunning = true;
UIManager Game::m_UIManager;


void Game::Init() {
	e_Window.Init();
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	e_DefaultShader.loadShader("defaultVertex.file", "defaultFragment.file");
	e_WaterShader.loadShader("waterVertex.file", "waterFragment.file");
	Proj = glm::mat4(1.0f);
	Proj = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 100000.0f);

	m_UIManager.Init(); //initialize the UI manager

	//after finishing loading all engines and setting engine's default rendering parameters, cache in and register the necessary objects for the game
	LoadAllModels();
	LoadAllTextures();
	RegisterAllBlocks();
	RegisterAllItems();

	//start the game loop
	GameLoop();
}
void Game::GameLoop() {

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	overworld = new Level();
	overworld->UpdateChunks(0, 0);
	player.SetPosition(10, 100, 10);
	player.AddStarterItems();
	CreatePlayerHud();

	//stuff for deltatime calculations
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;
	float deltaTime = 0.0;

	//for having the game run and for events
	SDL_Event event;
	//the actualy game loop
	while (IsGameRunning) { //the game loop
		//these 3 lines are for updating DeltaTime
		last = now;
		now = SDL_GetPerformanceCounter();
		deltaTime = ((now - last) * 1000) / SDL_GetPerformanceFrequency();

		while (SDL_PollEvent(&event)) {
			player.ProcessInput(event);
			if (event.type == SDL_QUIT) {
				CloseGame(); //quit
			}
		}
		player.Update(deltaTime);
		overworld->LevelUpdate(deltaTime);

		m_UIManager.Update(); //after updating world information update the UI information so that by the time rendering comes it will use current data


		//simply put set the clear color
		glClearColor(0.2, 1.0, 0.8, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//set the view matrix to the current camera's view
		View = player.getViewMatrix();

		//literally just render the entire world... lol
		e_LoadedTextures[0]->bind(); //binds the texture atlas just before drawing
		overworld->RenderLevel();
		e_LoadedTextures[0]->unbind();

		//render all the current UI stuff
		m_UIManager.Render();


		//swap buffers
		SDL_GL_SwapWindow(e_Window.m_Window);
	}

	delete overworld;

	//once the game stops terminate the game
	Terminate();
}
// terminates all engines for the game when the game closes and unloads every cached object
void Game::Terminate() {


	UnloadAllTextures();
	UnloadAllModels();
	e_Window.Termintate();
}

void Game::RegisterAllBlocks() { //register all block types in the hash map so they can be replicated for changing a block's type with custom parameters
	e_BlockRegistery[Air] = { { 0, 0, 0, 0, 0, 0 }, BlockVisiblity::Opaque }; //registers the AirBlock... not that it matters since Air has no physical appearance and is disregarded in the creation of a chunk but still
	e_BlockRegistery[Grass] = { { 0, 2, 3, 3, 3, 3 }, BlockVisiblity::Opaque }; //register the grass block
	e_BlockRegistery[Stone] = { { 1, 1, 1, 1, 1, 1 }, BlockVisiblity::Opaque }; //register the stone block
	e_BlockRegistery[Dirt] = { { 2, 2, 2, 2, 2, 2 }, BlockVisiblity::Opaque }; //register the dirt block
	e_BlockRegistery[Sand] = { { 18, 18, 18, 18, 18, 18 }, BlockVisiblity::Opaque }; //register the sand block
	e_BlockRegistery[Cobblestone] = { { 16, 16, 16, 16, 16, 16 }, BlockVisiblity::Opaque }; //register the cobblestone block
	e_BlockRegistery[WoodenPlanks] = { { 4, 4, 4, 4, 4, 4 }, BlockVisiblity::Opaque }; //register the wooden planks block
	e_BlockRegistery[Wood] = { { 21, 21, 20, 20, 20, 20 }, BlockVisiblity::Opaque }; //register the wood block
	e_BlockRegistery[Bedrock] = { { 17, 17, 17, 17, 17, 17 }, BlockVisiblity::Opaque }; //register the bedrock block
	e_BlockRegistery[Bricks] = { { 7, 7, 7, 7, 7, 7 }, BlockVisiblity::Opaque }; //register the bricks block
	e_BlockRegistery[CoalOre] = { { 34, 34, 34, 34, 34, 34 }, BlockVisiblity::Opaque }; //register the coal ore block
	e_BlockRegistery[IronOre] = { { 33, 33, 33, 33, 33, 33 }, BlockVisiblity::Opaque }; //register the iron ore block
	e_BlockRegistery[GoldOre] = { { 32, 32, 32, 32, 32, 32 }, BlockVisiblity::Opaque }; //register the gold ore block
	e_BlockRegistery[DiamondOre] = { { 50, 50, 50, 50, 50, 50 }, BlockVisiblity::Opaque }; //register the diamond ore block
	e_BlockRegistery[Glass] = { { 49, 49, 49, 49, 49, 49 }, BlockVisiblity::Opaque }; //register the glass block
	e_BlockRegistery[Obsidian] = { { 37, 37, 37, 37, 37, 37 }, BlockVisiblity::Opaque }; //register the obsidian block
	e_BlockRegistery[YellowFlower] = { { 13, 13, 13, 13, 13, 13 }, BlockVisiblity::Plant }; //register the red flower block
	e_BlockRegistery[RedFlower] = { { 12, 12, 12, 12, 12, 12 }, BlockVisiblity::Plant }; //register the red flower block
	e_BlockRegistery[WaterStill] = { { 205, 205, 205, 205, 205, 205 }, BlockVisiblity::Liquid }; //register the still water block
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

void Game::LoadAllModels() {// function is called at the start of the game to load everything in
	e_LoadedModels.push_back(InitModels::InitializeCube()); //the model for a block
}

void Game::UnloadAllModels() {//function is called when the game closes to unload everything out
	for (auto& n : e_LoadedModels) {
		n->DeleteModel();
		delete n;
	}
	e_LoadedModels.clear();
}

void Game::LoadAllTextures() {// function is called at the start of the game to load everything in
	e_LoadedTextures.push_back(new Texture("TextureAtlas.png")); //the texture atlas for the blocks
	e_LoadedTextures.push_back(new Texture("GUI/gui.png")); //texture atlas for GUI stuff like player hotbar and buttons
	e_LoadedTextures.push_back(new Texture("GUI/icons.png"));
	e_LoadedTextures.push_back(new Texture("GUI/inventory.png")); //the texture for the inventory
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

void Game::CreatePlayerHud() {
	m_UIManager.AddScreen(std::make_unique<PlayerHUDScreen>());
}
