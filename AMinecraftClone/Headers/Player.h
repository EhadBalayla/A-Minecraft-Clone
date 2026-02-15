#pragma once
#include "Entity.h"
#include "glm/gtc/matrix_transform.hpp"
#include "SDL.h"
#include "PlayerInventory.h"
#include "PauseMenu.h"
#include "InventoryItem.h"
#include "AABB.h"

class Player : public Entity
{
public:
	
	//a temporary function to add the starter items
	void AddStarterItems();

	Player();
	~Player();

	void Update(float DeltaTime) override;

	glm::mat4 getViewMatrix();
	glm::mat4 getViewOnlyMatrix();

	void ProcessInput(SDL_Event& e);
	
	void UpdateChunksAroundPlayer();

	glm::ivec2 GetCurrentChunkCoords();

	bool CreativeMode = true;
	bool IsPaused = false;
	bool IsInventory = false;

	//inventory stuff
	InventoryItem m_PlayerItems[36];
	InventoryItem m_TempItem; //used for when clicking on a slot
	InventoryItem m_CraftingItems[4]; //the crafting slots
	void AddItem(ItemType item, int amount);
	void RemoveItem(InventoryItem* item, int amountToRemove);

	BlockType GetRayBlock() const { return rayBlock; }
	glm::i64vec3 GetRayBlockPos() const { return rayBlockPos; }
private:
	const float cameraHeight = 1.62f;
	glm::dvec3 getCameraPosition();

	float mouseSensitivity = 0.5f;

	bool IsMovingForward = false;
	bool IsMovingBackward = false;
	bool IsMovingLeft = false;
	bool IsMovingRight = false;
	bool IsJumping = false;
	bool IsShifting = false;

	bool CanPerformAbility();



	//whatever the chunk the player is currently in
	int currentChunkX = 0;
	int currentChunkZ = 0;

	int currentLODChunkX[4] = { 0, 0, 0, 0 };
	int currentLODChunkZ[4] = { 0, 0, 0, 0 };

	//selected slot in hotbar
	uint8_t selectedSlot = 1; //ranges from 1 - 9, basically whatever slot you selct in hotbar
	void setSelectedSlot(uint8_t slot);

	void PlayerPlaceBlocks();


	bool ShouldUpdateChunks = true;
	const float CreativeSpeed = 0.1f;

	//cached ray
	BlockType rayBlock = BlockType::Air;
	glm::i64vec3 rayBlockPos = glm::i64vec3(0);
	glm::dvec3 rayHitPos = glm::dvec3(0.0);
};

#define CACHEDITEM Game::player.m_TempItem

