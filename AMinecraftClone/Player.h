#pragma once
#include "Entity.h"
#include "glm/gtc/matrix_transform.hpp"
#include "SDL.h"
#include "PlayerInventory.h"
#include "PauseMenu.h"
#include "InventoryItem.h"

class Player : public Entity
{
public:
	
	//a temporary function to add the starter items
	void AddStarterItems();

	Player();
	~Player();

	void Update(float DeltaTime) override;

	glm::mat4 getViewMatrix(); //returns the view matrix of the player for the rendering

	void ProcessInput(SDL_Event& e);
	
	void UpdateChunksAroundPlayer(); //for infinite render distance

	glm::ivec2 GetCurrentChunkCoords();
private:
	const float cameraHeight = 0.62f; //the height difference between the player's position and the camera
	glm::vec3 getCameraPosition(); //returns the camera position

	float mouseSensitivity = 0.5f; //mouse sensitivity

	//checks for wether player is pressing on certain direction
	bool IsMovingForward = false;
	bool IsMovingBackward = false;
	bool IsMovingLeft = false;
	bool IsMovingRight = false;
	bool IsJumping = false;
	bool IsShifting = false;

	bool CanPerformAbility(); //returns wether the player can perform an ability, as in wether any menu that should interrupt the player such as inventory is open

	//inventory stuff
	PlayerInventory* inventory;
	InventoryItem m_PlayerItems[36];
	void AddItem(Item item, int amount); //basically adds an item to the player's inventory,

	//Pause Menu stuff
	PauseMenu* m_PauseMenu;

	//whatever the chunk the player is currently in
	int currentChunkX = 0;
	int currentChunkZ = 0;

	//selected slot in hotbar
	uint8_t selectedSlot = 1; //ranges from 1 - 9, basically whatever slot you selct in hotbar
	void setSelectedSlot(uint8_t slot);

	void PlayerPlaceBlocks();


	bool CreativeMode = false;
	const float CreativeSpeed = 0.1f;
};

