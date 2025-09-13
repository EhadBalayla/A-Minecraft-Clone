#pragma once
#include "Window.h"
#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include "Model.h"
#include "Player.h"
#include "Texture.h"
#include "Level.h"
#include "UIManager.h"
#include <unordered_map>
#include "Item.h"

class Game
{
public: //this public section is simply for the members
	//just to clarify e_ is meant to represent the engine's members, because in the window class i already have an m_Window
	static Window e_Window; 
	static UIManager m_UIManager; // the manager of all UI in the game

	//the shaders
	static Shader e_DefaultShader;
	static Shader e_WaterShader;
	static Shader e_CloudShader;
	
	//object caching
	static std::vector<Model*> e_LoadedModels; //for entities and such, like player, zombie, or creeper for example
	static std::vector<Texture*> e_LoadedTextures; //for stuff like texture atlases

	//the basic camera for movement
	static Player player;

	static glm::mat4 Proj; //The Projection Matrix
	static glm::mat4 View; //The View Matrix;

	//the map... later on i will add THE NETHER [insert THE NETHER in jack black voice]
	static Level* overworld;

	//where we register the blocks, basically we call a function to register the blocks in this hashmap at the very start of the game
	static std::unordered_map<BlockType, BlockData> e_BlockRegistery;
	static std::unordered_map<ItemType, ItemData> e_ItemRegistery;

	void Init(); //initialize the engine
private: //this private section is for basic engine functions and initializations

	void GameLoop(); //the game loop
	void Terminate(); //ends the game when needed

	void RegisterAllBlocks(); //registers all the blocks into the game
	void RegisterAllItems(); //registers all items into the game

	void LoadAllModels(); //loads the models into memory
	void UnloadAllModels(); //Unloads the models from memory

	void LoadAllTextures(); //Loads all textures into memory
	void UnloadAllTextures(); //Unloasd all textures from memory

	static bool IsGameRunning;
public: //this "public" section is for gameplay functions such as closing game or creating a specific new UI screen and more
	static void CloseGame();
	static void CreatePlayerHud();

	const static int Radius_LOD0 = 8;
	const static int Radius_LOD1 = 8;
	const static int Radius_LOD2 = 8;
	const static int Radius_LOD3 = 8;
	const static int Radius_LOD4 = 8;

private: //this "private" section is for gameplay functions such as cloud rendering etc...
};

