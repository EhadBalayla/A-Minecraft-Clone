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
#include "AudioManager.h"
#include "DebugRenderer.h"
#include "DebugUIManager.h"

enum class GameState {
	MainMenu,
	InGame
};

class MainMenuScreen;
class Game
{
public:
	static Window e_Window; 
	static UIManager m_UIManager;
	static AudioManager m_AudioManager;
	static DebugRenderer m_DebugRenderer;
	static DebugUIManager m_DebugUI;

	
	static Shader e_OpaqueShader;
	static Shader e_PlantsShader;
	static Shader e_WaterShader;
	static Shader e_CloudShader;
	static Shader e_SkyColorShader;
	static Shader e_ChunkBorderShader;
	static unsigned int tempVAO; //temporary vao for stuff that have hardcoded shaders

	static bool ShowChunkBorder;
	static bool ShowDebugMenu;
	
	
	static std::vector<Model*> e_LoadedModels;
	static std::vector<Texture*> e_LoadedTextures;


	static Player player;

	static glm::mat4 Proj;
	static glm::mat4 View;

	//the map... later on i will add THE NETHER [insert THE NETHER in jack black voice]
	static Level* level;

	
	static std::unordered_map<BlockType, BlockData> e_BlockRegistery;
	static std::unordered_map<ItemType, ItemData> e_ItemRegistery;

	void Init();
	void GameLoop();
	void Terminate();
private:

	void RegisterAllBlocks();
	void RegisterAllItems();

	void LoadAllTextures();
	void UnloadAllTextures();

	static bool IsGameRunning;
public: //this "public" section is for gameplay functions such as closing game or creating a specific new UI screen and more
	static void CloseGame();


	const static uint8_t RenderDistance = 16;

	static void ChangeState(GameState newState);

private: //this "private" section is for gameplay functions such as cloud rendering etc...
	static GameState lastState;
	static GameState state;
	static void UnloadState(); //unloads the current state
	static void LoadState(); //loads the current state
	
	static void LoadMainMenuState();
	static void UnloadMainMenuState();

	static void LoadGameState();
	static void UnloadGameState();
};

