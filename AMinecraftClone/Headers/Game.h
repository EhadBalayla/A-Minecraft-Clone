#pragma once
#include "Window.h"
#include "AudioManager.h"
#include "DebugRenderer.h"
#include "DebugUIManager.h"
#include "NetworkingManager.h"

#include "Shader.h"
#include "Texture.h"

#include "PlayerHUDScreen.h"
#include "PlayerInventory.h"
#include "PauseMenu.h"
#include "MainMenuScreen.h"
#include "OptionsMenu.h"
#include "SavesMenu.h"

#include "Player.h"
#include <glm/glm.hpp>
#include "Level.h"
#include "Item.h"

#include <vector>
#include <unordered_map>

struct Glyph {
	int width;
	glm::vec2 uv0;
	glm::vec2 uv1;
};

enum class GameState {
	MainMenu,
	InGame
};

enum class MainMenuMenu {
	Title,
	SinglePlayer,
	MultiPlayer,
	Options,
};

enum class InGameMenu {
	PauseMenu,
	Options
};

class Game
{
public:
	static Window e_Window;
	static AudioManager m_AudioManager;
	static DebugRenderer m_DebugRenderer;
	static DebugUIManager m_DebugUI;
	static NetworkingManager m_Networking;

	
	static Shader e_OpaqueShader;
	static Shader e_PlantsShader;
	static Shader e_WaterShader;
	static Shader e_CloudShader;
	static Shader e_SkyColorShader;
	static Shader e_ChunkBorderShader;
	static Shader e_DummyPlayersShader;
	static Shader e_InventoryBlockShader;
	static Shader e_ImageWidgetShader;
	static Shader e_TextShader;
	static Shader e_MainMenuBackgroundShader;
	static unsigned int tempVAO;

	static Texture terrainAtlas;
	static Texture guiAtlas;
	static Texture iconsAtlas;
	static Texture inventoryTex;
	static Texture cloudsTex;
	static Texture fontTex;
	static Texture logoTex;
	static Texture dirtTex;

	static PlayerInventory inventoryMenu;
	static PlayerHUDScreen hudScreen;
	static PauseMenu pauseMenu;
	static MainMenuScreen mainMenuScreen;
	static OptionsMenu optionsMenu;
	static SavesMenu savesMenu;

	static bool ShowChunkBorder;
	static bool ShowDebugMenu;


	static Player player;

	static glm::mat4 ScreenProjection;
	static glm::mat4 Proj;
	static glm::mat4 View;

	//the map... later on i will add THE NETHER [insert THE NETHER in jack black voice]
	static Level* level;

	
	static std::unordered_map<BlockType, BlockData> e_BlockRegistery;
	static std::unordered_map<ItemType, ItemData> e_ItemRegistery;
	static std::unordered_map<char, Glyph> e_FontRegistery;

	void Init();
	void GameLoop();
	void Terminate();
private:

	void RegisterAllBlocks();
	void RegisterAllItems();
	void LoadFont();

	static bool IsGameRunning;
public: //this "public" section is for gameplay functions such as closing game or creating a specific new UI screen and more
	static void CloseGame();


	static int RenderDistance;
	static int LODCount; /*from 1 - 5*/

	static void ChangeState(GameState newState);

	static int m_ChosenTerrain;

	static MainMenuMenu MainMenuState;
	static InGameMenu InGameMenuState;

	static GameState GetGameState();
	static bool FirstClick;

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

